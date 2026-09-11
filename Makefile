STYLER    := stylet

SRC_DIR   := ./src
BUILD_DIR := ./build

test: always
	$(STYLER) $(SRC_DIR)/testvectors.c $(BUILD_DIR)/testvectors.c
	$(CC) -o $(BUILD_DIR)/test.out $(BUILD_DIR)/testvectors.c -I$(SRC_DIR)
	$(BUILD_DIR)/test.out

clean: always
	rm $(BUILD_DIR)/*

always:
	mkdir -p $(BUILD_DIR)
