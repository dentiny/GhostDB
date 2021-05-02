# compilation options
CC = g++
CFLAGS = -std=c++17 -g3 -Werror -Wall -pedantic -Wconversion -Wformat

# project directories
CUR_DIR  = $(shell pwd)
INC      = $(CUR_DIR)/include
INCLUDES = $(INC)/*.h
SRC      = $(CUR_DIR)/src
TEST     = $(CUR_DIR)/test
BUILD    = $(CUR_DIR)/build
LIB      = $(BUILD)/libghostdb.a
OBJS     = $(BUILD)/bloom.o                \
           $(BUILD)/buffer.o               \
           $(BUILD)/compaction_manager.o   \
           $(BUILD)/db.o                   \
           $(BUILD)/disk_manager.o         \
           $(BUILD)/level.o                \
           $(BUILD)/log_manager.o          \
					 $(BUILD)/recovery_manager.o     \
           $(BUILD)/run.o                  \
					 $(BUILD)/sstable_manager.o      \
					 $(BUILD)/sstable_page.o         \
					 $(BUILD)/util.o

all: $(BUILD)/test_db

$(BUILD)/test_db: $(TEST)/test_db.cc $(LIB)
	$(CC) $< $(CFLAGS) -lgtest -lgtest_main -pthread -I $(INC) $(LIB) -o $@

$(LIB): $(OBJS)
	ar -rc $@ $^

$(BUILD)/util.o: $(SRC)/util.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/buffer.o: $(SRC)/buffer.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/bloom.o: $(SRC)/bloom.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/run.o: $(SRC)/run.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/level.o: $(SRC)/level.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/sstable_page.o: $(SRC)/sstable_page.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/disk_manager.o: $(SRC)/disk_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/recovery_manager.o: $(SRC)/recovery_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/log_manager.o: $(SRC)/log_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -pthread -I $(INC) -c $< -o $@

$(BUILD)/compaction_manager.o: $(SRC)/compaction_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/sstable_manager.o: $(SRC)/sstable_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/db.o: $(SRC)/db.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean:
	rm -rf $(BUILD)/*
