#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <iterator>
#include <algorithm>

#include <iostream>
#include <ostream>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// Some rules for Pointer Wars 2025:
// 0. Implement all functions in linked_list.c
// 1. Feel free to add members to the structures, but please do not remove 
//    any or rename any. Doing so will cause test infrastructure to fail
//    to link against your shared library.
// 2. Same goes for the function declarations.
// 3. In your implementation, make use of malloc_fptr() and free_fptr()
//    functions for memory allocation and memory freeing. This allows
//    test infrastructure a bit more flexility. See linked_list.c for
//    declarations of those function pointers.


// A node in the linked_list structure.
// Feel free to change as desired.
//
struct node {
    node * next{NULL};
    unsigned int data{0};


    friend std::ostream& operator<<(std::ostream& out,const node& n){
        out<<n.data;
        return out;
    }
};



struct iterator {
    
    struct linked_list * ll;
    struct node * current_node;
    size_t current_index;
    unsigned int data;

    using iterator_category=std::forward_iterator_tag;
    using difference_type=std::ptrdiff_t;
    using value_type = node;
    using reference = node&;
    using pointer = node*;


public:
    iterator(pointer ptr=nullptr):current_node(ptr),current_index(0){
        if(ptr!=NULL){
            data=(ptr->data);
        }
    }

    reference operator*() const {return *current_node;}
    pointer operator->() const {return current_node;}

 
    iterator operator++() {
        if(current_node){
            current_node=current_node->next;
            data=current_node->data;
            ++current_index;
        }

        return *this;
    }

    iterator operator++(int){
        iterator temp=*this;
        ++(*this);
        return temp;
    }

    friend bool operator==(const iterator& a,const iterator& b){return a.current_node==b.current_node;}
    friend bool operator!=(const iterator& a,const iterator& b){return a.current_node!=b.current_node;}


};


struct linked_list {
    struct node * head;
    struct node * tail;
    linked_list()=default;
    linked_list(node* n):head(n),tail(n){}
    
    //begin and end for iterator
    ////end has NULL to enable iteration until the end
    iterator begin(){return iterator(head);};
    iterator end(){return iterator(NULL);}
};

void show(linked_list * ll);
// Creates a new linked_list.
// PRECONDITION: Register malloc() and free() functions via the
//               linked_list_register_malloc() and 
//               linked_list_register_free() functions.
// POSTCONDITION: An empty linked_list has its head point to NULL.
// Returns a new linked_list on success, NULL on failure.
//
struct linked_list * linked_list_create(void);

// Deletes a linked_list.
// \param ll : Pointer to linked_list to delete
// POSTCONDITION : An empty linked_list has its head point to NULL.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete(struct linked_list * ll);

// Returns the size of a linked_list.
// \param ll : Pointer to linked_list.
// Returns size on success, SIZE_MAX on failure.
//
size_t linked_list_size(struct linked_list * ll);

// Inserts an element at the end of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_end(struct linked_list * ll,
                            unsigned int data);

// Inserts an element at the front of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_front(struct linked_list * ll,
                              unsigned int data);

// Inserts an element at a specified index in the linked_list.
// \param ll    : Pointer to linked_list.
// \param index : Index to insert data at.
// \param data  : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert(struct linked_list * ll,
                        size_t index,
                        unsigned int data);

// Finds the first occurrence of data and returns its index.
// \param ll   : Pointer to linked_list.
// \param data : Data to find.
// Returns index of the first index with that data, SIZE_MAX otherwise.
//
size_t linked_list_find(struct linked_list * ll,
                        unsigned int data);

// Removes a node from the linked_list at a specific index.
// \param ll    : Pointer to linked_list.
// \param index : Index to remove node.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_remove(struct linked_list * ll,
                        size_t index);

// Creates an iterator struct at a particular index.
// \param linked_list : Pointer to linked_list.
// \param index       : Index of the linked list to start at.
// Returns pointer to an iterator on success, NULL otherwise.
//
struct iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index);

// Deletes an iterator struct.
// \param iterator : Iterator to delete.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete_iterator(struct iterator * iter);

// Iterates to the next node in the linked_list.
// \param iterator: Iterator to iterate on.
// Returns TRUE when next node is present, FALSE once end of list is reached.
//
bool linked_list_iterate(struct iterator * iter);

// Registers malloc() function.
// \param malloc : Function pointer to malloc()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_malloc(void * (*malloc)(size_t));

// Registers free() function.
// \param free : Function pointer to free()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_free(void (*free)(void*));

#endif
