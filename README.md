# Projet SecMalloc

## Introduction

Ce projet fournit une implémentation personnalisée des fonctions d'allocation de mémoire sécurisée, notamment `malloc` et `free`, en utilisant `mmap` pour la gestion de la mémoire au niveau utilisateur. Ce projet est conçu pour être utilisé dans des environnements nécessitant un haut niveau de sécurité dans la gestion de la mémoire.
C'était bien dur.

## Structure du Projet

Le projet est organisé comme suit :

- `src/` : Contient les fichiers source `.c` de l'implémentation.
- `include/` : Contient les fichiers d'en-tête `.h`.
- `lib/` : Contient les bibliothèques compilées, statiques et dynamiques.
- `obj/` : Contient les fichiers objet intermédiaires de la compilation.
- `test/` : Contient les tests unitaires et leurs exécutables.


### Compilation de la Bibliothèque Statique

Pour compiler la bibliothèque statique 'libmy_secmalloc.a' :

$make static

Pour compiler la bibliothèque dynamique 'libmy_secmalloc.so':

$make clean dynamic

Pour compiler les tests en dynamique : (Veuillez changer la varible d'environnement avec : export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH ./test/test_my_malloc_dynamic)

$make test_dynamic 

puis executer le test test_my_malloc_dynamic

Pour compiler les test en statique : 

$make test_static

puis excuter le test test_my_malloc_static

#### Explication des fonctionalités et de l'algorithmie : 

-Creation des pool_data et pool_meta :

Les deux pools de données différents ont été alloués tous deux via `mmap`.

- Le pool de données est utilisée pour les allocations mémoire demandées par l'utilisateur tandis que le pool métadonnées garde une trace des bloc alloués incluant leur adresse leur taille leur état et des canarys.

- Ces pool sont crées avec une fonction d'initiation (qui contient le mmap) en structurant en bloc l'espace fourni par le nmap.

- La fonction my_malloc alloue de la mémoire en parcourant le pool de métadonnées pour trouver un bloc libre et de taille suffisante.
----Lorsqu'un tel bloc est trouvé, il est marqué comme occupé et l'adresse du début du bloc dans le début de données et retourné à l'utilisateur.
----Si aucun bloc adéquat n'est disponible, le programme retourne un message prévenant qu'il n'y a pas de blocs satisfaisant son désir.

- La fonction my_free libère la mémoire précédemment allouée par my_malloc.
----La fonction calcule l'offset du pointeur dans le pool de données pour identifier l'indice du bloc à libérer.
----Si l'indice est hors limites, cela signifie que l'adresse à libérer n'est pas valide, et une erreur est signalée.
----Avant de libérer le bloc, la fonction vérifie si celui-ci est déjà libre, évitant ainsi les doubles libérations
----Elle contrôle également l'intégrité des canaris de début et de fin pour détecter toute corruption de mémoire qui aurait pu survenir pendant l'utilisation du bloc.
----Si les canaris ne correspondent pas aux valeurs attendues, une erreur de corruption de mémoire est signalée, et la fonction se termine sans libérer le bloc pour éviter d'aggraver une éventuelle corruption de mémoire.
----Si tout est en ordre, la fonction parcourt les métadonnées pour marquer le bloc et tous les blocs consécutifs nécessaires (dans le cas d'allocations s'étendant sur plusieurs blocs) comme libres, réinitialisant leur statut dans le pool de métadonnées.
--------- Elle affiche enfin un message confirmant le nombre de blocs libérés et l'adresse de départ du bloc libéré, fournissant un retour visuel de l'action de libération.

##### Explication des tests 

Le fichier test fournies présente différente chose : 

-- Allocations réussies : Trois blocs de mémoire de tailles différentes sont alloués, démontrant la capacité de la bibliothèque à gérer des demandes d'allocation variées.
-- Double libération : Un bloc de mémoire est intentionnellement libéré deux fois pour tester la réaction de la bibliothèque face à une double libération, un cas d'usage erroné courant en gestion de mémoire.
Dépassement de mémoire : Un bloc est rempli avec plus de données que la taille allouée(avec memset), simulant un dépassement de tampon pour vérifier si la bibliothèque détecte et gère correctement cette violation
