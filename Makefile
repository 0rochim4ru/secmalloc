CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -fPIC
AR := ar
ARFLAGS := rcs
LD := gcc
LDFLAGS := -shared
RM := rm -f
MKDIR := mkdir -p

SRC_DIR := src
OBJ_DIR := obj
LIB_DIR := lib
TEST_DIR := test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

STATIC_LIB := $(LIB_DIR)/libmy_secmalloc.a
DYNAMIC_LIB := $(LIB_DIR)/libmy_secmalloc.so

.PHONY: all clean static dynamic test_static test_dynamic clean_static clean_dynamic

all: static dynamic test_static test_dynamic

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	$(MKDIR) $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

dynamic: $(DYNAMIC_LIB)

$(DYNAMIC_LIB): $(OBJS)
	$(MKDIR) $(LIB_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

test_static: $(STATIC_LIB)
	$(CC) $(CFLAGS) -o $(TEST_DIR)/test_my_malloc_static $(TEST_DIR)/test_my_malloc.c -L$(LIB_DIR) -l:libmy_secmalloc.a

test_dynamic: $(DYNAMIC_LIB)
	$(CC) $(CFLAGS) -o $(TEST_DIR)/test_my_malloc_dynamic $(TEST_DIR)/test_my_malloc.c -L$(LIB_DIR) -lmy_secmalloc -Wl,-rpath,$(LIB_DIR)

clean:
	$(RM) -r $(OBJ_DIR)/* $(LIB_DIR)/* $(TEST_DIR)/test_my_malloc_static $(TEST_DIR)/test_my_malloc_dynamic

clean_static:
	$(RM) $(STATIC_LIB) $(OBJ_DIR)/*.o
	make static

clean_dynamic:
	$(RM) $(DYNAMIC_LIB) $(OBJ_DIR)/*.o
	make dynamic

dynamic: $(DYNAMIC_LIB)

$(DYNAMIC_LIB): $(OBJS)
	$(MKDIR) $(LIB_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

