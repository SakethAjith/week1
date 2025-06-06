#include "linked_list.h"
#include <cstdint>
#include <stdlib.h>
// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = malloc;
static void   (*free_fptr)(void* addr)    = free; 


linked_list * linked_list_create(void){
       linked_list * l=(linked_list*)malloc_fptr(sizeof(linked_list));
    if (l==NULL){
        return nullptr;
    }
    l->head=NULL;
    // isn't this unsafe?
    // if l was a temporar variable it would be a dandling pointer
    // but since malloc allocates on stack this is now ok?
    return l;
}

// Deletes a linked_list.
// \param ll : Pointer to linked_list to delete
// POSTCONDITION : An empty linked_list has its head point to NULL.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete(linked_list * ll){
    node* prev=NULL;
    while (ll->head){
        prev=ll->head;
        ll->head=ll->head->next;
        free(prev);
    }
    return ll->head==NULL;
//    return true;
}

// Returns the size of a linked_list.
// \param ll : Pointer to linked_list.
// Returns size on success, SIZE_MAX on failure.
//
size_t linked_list_size(linked_list * ll){
    int count{0};
    auto temp=ll->head;
    while (temp) {
        ++count;
        temp=temp->next;
    }
    return count;
}

// Inserts an element at the end of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_end(linked_list * ll,
                            unsigned int data){
    if(ll==NULL){
        ll=(linked_list*)malloc_fptr(sizeof(linked_list));
    }
    node* temp=(node*)malloc_fptr(sizeof(node));
    temp->data=data;
    

    if (ll->head==NULL){
        ll->head=temp;
        return true;
    }



    if(ll->tail==NULL){
        auto temp=ll->head;
        while (temp->next){
            temp=temp->next;
        }
        ll->tail=temp;
    }

    ll->tail->next=temp;
    ll->tail=temp;


    return ll->tail->data==data;
}


// Inserts an element at the front of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_front(struct linked_list * ll,
                              unsigned int data){
    node* temp=(node*)malloc_fptr(sizeof(node));
    temp->data=data;
    temp->next=ll->head;
    ll->head=temp;
    return ll->head->data==data;
}

// Inserts an element at a specified index in the linked_list.
// \param ll    : Pointer to linked_list.
// \param index : Index to insert data at.
// \param data  : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert(struct linked_list * ll,
                        size_t index,
                        unsigned int data){
    try{
        node* temp=(node*)malloc_fptr(sizeof(node));
        node* inserted=(node*)malloc_fptr(sizeof(node));
        temp=ll->head;
        for(size_t i=0;i<index;++i){
           temp=temp->next; 
        }

        inserted->data=data;
        inserted->next=temp->next;
        temp->next=inserted;
    }catch(int){
        return false;
    }
    return true;

}

// Finds the first occurrence of data and returns its index.
// \param ll   : Pointer to linked_list.
// \param data : Data to find.
// Returns index of the first index with that data, SIZE_MAX otherwise.
//
size_t linked_list_find(struct linked_list * ll,
                        unsigned int data){
    size_t pos{0};
    auto temp=ll->head;
    while(temp){
        ++pos;
        if (temp->data==data){
            return pos;
        }
        temp=temp->next;
    }
    return SIZE_MAX;
}

// Removes a node from the linked_list at a specific index.
// \param ll    : Pointer to linked_list.
// \param index : Index to remove node.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_remove(struct linked_list * ll,
                        size_t index){

    try{
        size_t pos{0};
        auto temp=ll->head;
        while(temp){
            if (pos==index-1){
                break;
            }
            temp=temp->next;
            ++pos;
        }
        auto todel=temp->next;
        temp->next=temp->next->next;
        todel->next=NULL;
        free_fptr(todel);
        return todel==NULL; 
    }catch(int){
        return false;
    }
    return true;
}





// Creates an iterator struct at a particular index.
// \param linked_list : Pointer to linked_list.
// \param index       : Index of the linked list to start at.
// Returns pointer to an iterator on success, NULL otherwise.
//
//
iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index){
    if(ll==NULL || ll->head==NULL){
        return NULL;
    }   

    iterator* iter=new iterator(ll->head);
    iter->ll=ll;

    while (iter->current_node!=nullptr && static_cast<size_t>(iter->current_index)!=(index)){
        ++(*iter);
    }
    return iter;
}


// Deletes an iterator struct.
// \param iterator : Iterator to delete.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete_iterator(struct iterator * iter){
    free_fptr(iter);
    return iter==nullptr;
}

// Iterates to the next node in the linked_list.
// \param iterator: Iterator to iterate on.
// Returns TRUE when next node is present, FALSE once end of list is reached.
//
bool linked_list_iterate(struct iterator * iter){
    if(iter->current_node==NULL){
        return false;
    }
    if(iter->current_node->next!=NULL){
        ++(*iter);
    }
    return true;
}

// Registers malloc() function.
// \param malloc : Function pointer to malloc()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_malloc(void * (*malloc)(size_t)){
    if(malloc==NULL){
        return false;
    }
    malloc_fptr = malloc;
    return true;
}

// Registers free() function.
// \param free : Function pointer to free()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_free(void (*free)(void*)){
    if (free==NULL){
        return false;
    }
    free_fptr = free;
    return true;
}


