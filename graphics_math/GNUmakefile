CC = gcc
CPPFLAGS = -I./include -I/usr/local/include
CFLAGS = -std=c11 -pedantic -Wall -Wextra -g -O0
LDFLAGS = -lm

SRC_DIR = src
SRCS = $(addprefix $(SRC_DIR)/,vector.c)
OUTPUT_DIR = build_output
OUTPUT_DIR_MARKER = $(OUTPUT_DIR)/.f
OBJS = $(subst $(SRC_DIR)/,$(OUTPUT_DIR)/,$(patsubst %.c,%.o,$(SRCS)))

TEST_SRC_DIR = tests
TEST_OUTPUT_DIR = $(OUTPUT_DIR)/tests
TEST_OUTPUT_DIR_MARKER = $(TEST_OUTPUT_DIR)/.f
TEST_PROGRAMS = $(addprefix $(TEST_OUTPUT_DIR)/,check_vector)
TEST_OBJS = $(addsuffix .o,$(TEST_PROGRAMS))
ifeq ($(CC),clang)
TEST_CFLAGS = -Wno-gnu-zero-variadic-macro-arguments
endif
TEST_LDFLAGS = -L/usr/local/lib -lcheck

.PHONY: all clean print test

all: $(OBJS)

$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.c $(OUTPUT_DIR_MARKER)
	$(COMPILE.c) $< -o $@

$(OUTPUT_DIR_MARKER):
	mkdir -p $(dir $@)
	touch $@

test: $(OBJS) $(TEST_PROGRAMS)
	$(TEST_OUTPUT_DIR)/check_vector

$(TEST_OUTPUT_DIR)/check_vector: $(TEST_OUTPUT_DIR)/check_vector.o $(OBJS) $(TEST_OUTPUT_DIR_MARKER)
	$(LINK.c) -o $@ $(OBJS) $< $(TEST_LDFLAGS)

$(TEST_OUTPUT_DIR)/%.o: $(TEST_SRC_DIR)/%.c $(TEST_OUTPUT_DIR_MARKER)
	$(COMPILE.c) $(TEST_CFLAGS) $< -o $@

$(TEST_OUTPUT_DIR_MARKER): $(OUTPUT_DIR_MARKER)
	mkdir -p $(dir $@)
	touch $@

clean:
	rm -rf $(OUTPUT_DIR)

print:
	$(info SRCS -> $(SRCS))
	$(info OBJS -> $(OBJS))
	$(info TEST_OUTPUT_DIR -> $(TEST_OUTPUT_DIR))
	$(info MAKE_VERSION -> $(MAKE_VERSION))
	$(info .FEATURES -> $(.FEATURES))
	$(info COMPILE.c -> $(COMPILE.c))
	$(info CC -> $(CC))
	$(info LINK.c -> $(LINK.c))
