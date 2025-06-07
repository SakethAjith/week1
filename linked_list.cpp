#include "linked_list.h"
#include <cstdint>
#include <stdlib.h>

#define PRINT(x,y) printf(#x #y);
// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = malloc;
static void   (*free_fptr)(void* addr)    = free; 


void show(linked_list * ll){
    for(iterator i=ll->begin();i!=ll->end();++i){
        PRINT("\t",$(i.data));
    }
    PRINT("\n",-1);
}

linked_list * linked_list_create(void){
       linked_list * l=(linked_list*)malloc_fptr(sizeof(linked_list));
    if (l==NULL){
        return nullptr;
    }
    l->head=NULL;

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
        free_fptr(prev);
    }
    return ll->head==NULL;
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
        return false;
    }
    node* temp=(node*)malloc_fptr(sizeof(node));
    temp->data=data;
    
    if (ll->head==NULL){
        ll->head=temp;
        ll->tail=temp;
        return true;
    }

    if(ll->tail==NULL || ll->tail==ll->head){
        auto iter=ll->head;
        while (iter->next){
            iter=iter->next;
        }
        ll->tail=iter;
    }


    ll->tail->next=temp;
    ll->tail=ll->tail->next;

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

    if(ll==NULL){
        //linked list null
        return false;
    }


    node* temp=(node*)malloc_fptr(sizeof(node));
    temp=ll->head;
    for(size_t i=0;i<index;++i){
        if(temp && temp->next){
            temp=temp->next;
            continue;
        }
        if (temp->next==NULL && index-i>1){
            return false;
        }
    }
    
    node* inserted=(node*)malloc_fptr(sizeof(node));
    inserted->data=data;
    inserted->next=temp->next;
    temp->next=inserted;
    return true;

}

// Finds the first occurrence of data and returns its index.
// \param ll   : Pointer to linked_list.
// \param data : Data to find.
// Returns index of the first index with that data, SIZE_MAX otherwise.
//
size_t linked_list_find(struct linked_list * ll,
                        unsigned int data){
    if(ll==NULL || ll->head==NULL){
        return false;
    }
    if(data==ll->head->data){

        return 0;
    }
     
    size_t pos{0};

    for(auto i: *ll){
        if(i.data==data){
            return pos;
        }
        ++pos;
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

    if(ll==NULL || ll->head==NULL){
        return false;
    }

    if(index==0){
        auto temp=ll->head;
        ll->head=ll->head->next;
        temp->next=NULL;
        free_fptr(temp);
        return true;
    }

    size_t pos{0};
    auto temp=ll->head;

    while(temp){
        if (pos==index-1){
            break;
        }
        temp=temp->next;
        ++pos;
    }

    if(index-pos>1){
        PRINT("cause","1"); 
        return false;
    }

    if(temp==NULL || temp->next==NULL){
        PRINT("cause","2"); 
        return false;
    }

    auto todel=temp->next;
    temp->next=temp->next->next;
    todel->next=NULL;
    free_fptr(todel);
    todel=NULL;
    if(todel!=NULL){
        PRINT("cause","3");
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
    if(iter==nullptr){
        return false;
    }
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


