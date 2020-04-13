CC				= gcc
CFLAGS			= -g
LIB_DIR			= ./lib
DEPS			= yasML.h

TEST_DIR		= ./tests
TEST_INC_DIR	= -I./lib/unity
TEST_LIBS		= -lm

test: multiply_test identity_test
	$(TEST_DIR)/multiply_test
	$(TEST_DIR)/identity_test

multiply_test: unity.o yasML.h $(TEST_DIR)/multiply.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $^ $(TEST_INC_DIR) $(TEST_LIBS)

identity_test: unity.o yasML.h $(TEST_DIR)/identity.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $^ $(TEST_INC_DIR) $(TEST_LIBS)

unity.o: lib/unity/unity.c
	$(CC) -c $(LIB_DIR)/unity/unity.c $(INC_DIR)

.PHONY: clean test

clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TEST_DIR)/*_test