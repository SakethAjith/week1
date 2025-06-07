#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "linked_list.h"

#define PRINT(x) printf(#x);
#define TEST(x) printf("Running test " #x "\n"); fflush(stdout);
#define SUBTEST(x) printf("    Executing subtest " #x "\n"); fflush(stdout); \
                   alarm(1);
#define FAIL(cond, msg) if (cond) {\
                        printf("    FAIL! "); \
                        printf(#msg "\n"); \
                        exit(-1);\
                        }
#define PASS(x) printf("PASS!\n"); alarm(0);

bool instrumented_malloc_fail_next             = false;
bool instrumented_malloc_last_alloc_successful = false;

void * instrumented_malloc(size_t size) {
    if (instrumented_malloc_fail_next) {
        instrumented_malloc_fail_next             = false;
	instrumented_malloc_last_alloc_successful = false;
	return NULL;
    }

    void * ptr = malloc(size);
    instrumented_malloc_last_alloc_successful = (ptr != NULL);

    return ptr;
}

void check_node_delete(void){

    TEST("test delete node begin");
    linked_list * ll = linked_list_create();
    for(int i=1;i<=5;i++){
        linked_list_insert_end(ll, i);
    }

    SUBTEST("delete first node");
    auto result=linked_list_remove(ll, 0);
    FAIL((!result && 5-linked_list_size(ll)==1)," failed to delete first node!\n");
    
    int count=linked_list_size(ll);
    SUBTEST("delete last node");
    result = linked_list_remove(ll, count-1);
    FAIL((!result && 4-linked_list_size(ll)==1)," failed to delete last node!\n");

    SUBTEST("delete middle node");
    result= linked_list_remove(ll, 1);
    FAIL((!result && 3-linked_list_size(ll)==1)," failed to delete 3rd node!\n");

    PASS("deleting nodes pass!\n");
    linked_list_delete(ll);
}

void check_node_find(void){
    TEST("test find node begin");
    linked_list * ll = linked_list_create();

    
    for(int i=1;i<=5;++i){
        linked_list_insert_front(ll, i);
    }


    SUBTEST("Find first element");
    auto result=linked_list_find(ll, 1);
    FAIL((result!=4), "failed finding first element!\n");

    SUBTEST("Find last element");
    result=linked_list_find(ll, static_cast<size_t>(5));
    FAIL((result!=0), "failed finding last element!\n");

    SUBTEST("Find middle element");
    result=linked_list_find(ll, static_cast<size_t>(3));
    FAIL((result!=2), "failed finding middle element!\n");

    PASS("finding nodes pass!\n");
    linked_list_delete(ll);

}



int main(){

    linked_list_register_malloc(&instrumented_malloc);
    linked_list_register_free(&free);

    check_node_delete();
    check_node_find();
}
