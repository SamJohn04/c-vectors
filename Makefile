STYLER    := stylet

SRC_DIR   := ./src
BUILD_DIR := ./build

.PHONY: test clean

test: always
	$(STYLER) $(SRC_DIR)/testvectors.c $(BUILD_DIR)/testvectors.c

	# DEBUG

	$(CC) -o $(BUILD_DIR)/test.out $(BUILD_DIR)/testvectors.c -I$(SRC_DIR)
	$(BUILD_DIR)/test.out

	# PROD

	$(CC) -o $(BUILD_DIR)/test-prod.out $(BUILD_DIR)/testvectors.c -I$(SRC_DIR) -DNDEBUG
	$(BUILD_DIR)/test-prod.out

clean: always
	rm $(BUILD_DIR)/*

always:
	mkdir -p $(BUILD_DIR)
