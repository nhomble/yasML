CC				= gcc
CFLAGS			= -g
LIB_DIR			= ./lib
DEPS			= yasML.h

TEST_DIR		= ./tests
TEST_INC_DIR	= -I./lib/unity/src
TEST_LIBS		= -lm

test: multiply_test identity_test readme_test
	$(TEST_DIR)/multiply_test
	$(TEST_DIR)/identity_test
	$(TEST_DIR)/readme_test

multiply_test: unity.o yasML.h $(TEST_DIR)/multiply.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $^ $(TEST_INC_DIR) $(TEST_LIBS)

identity_test: unity.o yasML.h $(TEST_DIR)/identity.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $^ $(TEST_INC_DIR) $(TEST_LIBS)

readme_test: unity.o yasML.h $(TEST_DIR)/readme.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $^ $(TEST_INC_DIR) $(TEST_LIBS)

unity.o: lib/unity/src/unity.c
	$(CC) -c $(LIB_DIR)/unity/src/unity.c $(INC_DIR)

.PHONY: clean test

clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TEST_DIR)/*_test
