# compilation options
CC = g++
CFLAGS = -std=c++17 -Werror -Wall -pedantic -Wconversion -Wformat

# project directories
CUR_DIR  = $(shell pwd)
INC      = $(CUR_DIR)/include
INCLUDES = $(INC)/*.h
SRC      = $(CUR_DIR)/src
TEST     = $(CUR_DIR)/test
BUILD    = $(CUR_DIR)/build
OBJS     = $(BUILD)/bloom.o                \
           $(BUILD)/buffer.o               \
           $(BUILD)/buffer_pool_manager.o  \
           $(BUILD)/compaction_manager.o   \
           $(BUILD)/db.o                   \
           $(BUILD)/disk_manager.o         \
           $(BUILD)/fs_util.o              \
           $(BUILD)/level.o                \
           $(BUILD)/log_manager.o          \
           $(BUILD)/run.o                  \
           $(BUILD)/string_util.o

all: $(OBJS) $(BUILD)/test_db

$(BUILD)/test_db: $(TEST)/test_db.cc $(OBJS)
	$(CC) $(CFLAGS) -pthread -I $(INC) $^ -o $@

$(BUILD)/fs_util.o: $(SRC)/fs_util.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/string_util.o: $(SRC)/string_util.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/buffer.o: $(SRC)/buffer.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/bloom.o: $(SRC)/bloom.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/run.o: $(SRC)/run.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/level.o: $(SRC)/level.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/disk_manager.o: $(SRC)/disk_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/log_manager.o: $(SRC)/log_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -pthread -I $(INC) -c $< -o $@

$(BUILD)/compaction_manager.o: $(SRC)/compaction_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/buffer_pool_manager.o: $(SRC)/buffer_pool_manager.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

$(BUILD)/db.o: $(SRC)/db.cc $(INCLUDES)
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean:
	rm $(BUILD)/*
