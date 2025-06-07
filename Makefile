# Welcome to the world's worst Makefile.
CC := g++

WARNINGS_ARE_ERRORS := -Wall -Wextra -Werror
COMPILER_OPTIMIZATIONS := -O3 -g
SO_FLAGS := -shared -fPIC -g 
CFLAGS := $(WARNINGS_ARE_ERRORS) $(COMPILER_OPTIMIZATIONS)

# Add any source files that you need to be compiled
# for your linked list here.
#
LINKED_LIST_SOURCE_FILES := linked_list.cpp
LINKED_LIST_OBJECT_FILES := linked_list.o

# Functional testing support
#
FUNCTIONAL_TEST_SOURCE_FILES := linked_list_test_program.cpp
FUNCTIONAL_TEST_OBJECT_FILES := linked_list_test_program.o

EXTENDED_TEST_SOURCE_FILES := extended_tests.cpp
EXTENDED_TEST_OBJECT_FILES := extended_tests.o

liblinked_list.so : $(LINKED_LIST_OBJECT_FILES)
	$(CC) $(CFLAGS) $(SO_FLAGS) $^ -o $@

linked_list_test_program: liblinked_list.so $(FUNCTIONAL_TEST_OBJECT_FILES)
	$(CC) -o $@ $(FUNCTIONAL_TEST_OBJECT_FILES)  -L `pwd` -llinked_list

run_functional_tests: linked_list_test_program
	LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH ./linked_list_test_program

run_functional_tests_gdb: linked_list_test_program
	LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH gdb ./linked_list_test_program


# Not part of original

extended_tests: liblinked_list.so 
	$(CC) $(CFLAGS) $(SO_FLAGS) $(EXTENDED_TEST_SOURCE_FILES) -o $(EXTENDED_TEST_OBJECT_FILES)
#	$(CC) $(CFLAGS) $(SO_FLAGS) $(EXTENDED_TEST_OBJECT_FILES) -o $@
	$(CC) -o $@ $(EXTENDED_TEST_OBJECT_FILES)  -L `pwd` -llinked_list

	
run_extended_functional_tests: extended_tests 
	LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH  ./extended_tests

run_extend: 
	LD_LIBRARY_PATH=`pwd`:$LD_LIBRARY_PATH  ./extended_tests

%.o : %.cpp
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm $(LINKED_LIST_OBJECT_FILES) $(FUNCTIONAL_TEST_OBJECT_FILES) $(EXTENDED_TEST_OBJECT_FILES) liblinked_list.so linked_list_test_program extended_tests
