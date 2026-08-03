CC := gcc

SRC_DIR   := ./src
BUILD_DIR := ./build

test: always
	$(CC) -o $(BUILD_DIR)/test.out $(SRC_DIR)/testvectors.c
	$(BUILD_DIR)/test.out

clean: always
	rm $(BUILD_DIR)/test.out

always:
	mkdir -p $(BUILD_DIR)
