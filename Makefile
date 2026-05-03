CC				= gcc
CFLAGS			= -g
LIB_DIR			= ./lib
DEPS			= yasML.h

TEST_DIR		= ./tests
TEST_INC_DIR	= -I./lib/unity/src
TEST_LIBS		= -lm

test: multiply_test identity_test readme_test zero_vector_test reduce_test gram_schmidt_test projection_test inversion_test
	$(TEST_DIR)/multiply_test
	$(TEST_DIR)/identity_test
	$(TEST_DIR)/readme_test
	$(TEST_DIR)/zero_vector_test
	$(TEST_DIR)/reduce_test
	$(TEST_DIR)/gram_schmidt_test
	$(TEST_DIR)/projection_test
	$(TEST_DIR)/inversion_test

multiply_test: unity.o yasML.h $(TEST_DIR)/multiply.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

identity_test: unity.o yasML.h $(TEST_DIR)/identity.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

readme_test: unity.o yasML.h $(TEST_DIR)/readme.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

zero_vector_test: unity.o yasML.h $(TEST_DIR)/zero_vector.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

reduce_test: unity.o yasML.h $(TEST_DIR)/reduce.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

gram_schmidt_test: unity.o yasML.h $(TEST_DIR)/gram_schmidt.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

projection_test: unity.o yasML.h $(TEST_DIR)/projection.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

inversion_test: unity.o yasML.h $(TEST_DIR)/inversion.c
	$(CC) $(CFLAGS) -o $(TEST_DIR)/$@ $(filter-out %.h,$^) $(TEST_INC_DIR) $(TEST_LIBS)

unity.o: lib/unity/src/unity.c
	$(CC) -c $(LIB_DIR)/unity/src/unity.c $(INC_DIR)

.PHONY: clean test

clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(TEST_DIR)/*_test
