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
OBJS     = $(BUILD)/bloom.o                \
           $(BUILD)/buffer.o               \
           $(BUILD)/compaction_manager.o   \
           $(BUILD)/db.o                   \
           $(BUILD)/disk_manager.o         \
           $(BUILD)/level.o                \
           $(BUILD)/log_manager.o          \
					 $(BUILD)/page.o                 \
           $(BUILD)/run.o                  \
					 $(BUILD)/sstable_manager.o      \
					 $(BUILD)/util.o

all: $(OBJS) $(BUILD)/test_db

$(BUILD)/test_db: $(TEST)/test_db.cc $(OBJS)
	$(CC) $(CFLAGS) -pthread -I $(INC) $^ -o $@

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

$(BUILD)/page.o: $(SRC)/page.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/disk_manager.o: $(SRC)/disk_manager.cc $(INCLUDES)
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
	rm $(BUILD)/*
