#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bucket_item {
    char* word ; 
    size_t index ; 
    struct bucket_item* next_item ; 
} ; 

struct hashtable {
    struct bucket_item* buckets ; 
    size_t num_buckets ;  
    size_t num_items ; 
} hashtable ; 

struct hashtable* hashtable_create( size_t num_buckets ) {
    struct hashtable* table = (struct hashtable*) malloc( sizeof( hashtable ) ) ;
    table -> buckets = (struct bucket_item*) calloc( num_buckets , sizeof( struct bucket_item ) ) ; 
    table -> num_buckets = num_buckets ;
    table -> num_items = 0 ; 
    return table ; 
}

size_t hash( 
    struct hashtable* table , 
    const char* word 
) {
    size_t h = 0 ; 
    for( size_t i = 0 ; i < strlen( word ) ; i++ ) {
        h = h * 31 + (size_t)word[ i ] ;
    }
    return h % table -> num_buckets ;  
}

// FOR DEBUGGING PURPOSES
void hashtable_print( 
    struct hashtable* table 
) {
    for( size_t i = 0 ; i < table -> num_buckets ; i++ ) {
        struct bucket_item* curr_node = (table -> buckets[i]).next_item ; 
        printf( "%li " , i ) ; 
        while( curr_node ) {
            printf( "%s " , curr_node -> word ) ; 
            curr_node = curr_node -> next_item ; 
        }
        printf( "\n" ) ; 
    }
}

void hashtable_release( struct hashtable* table ) {
    for( size_t i = 0 ; i < table -> num_buckets ; i++ ) {
        struct bucket_item* curr_node = (table -> buckets[i]).next_item ; 
        while( curr_node ) {
            struct bucket_item* next_item = curr_node -> next_item ; 
            free( curr_node -> next_item ) ; 
            curr_node = next_item ; 
        }
    }
    free( table -> buckets ) ; 
    free( table ) ;
}

void hashtable_insert( 
    struct hashtable* table , 
    const char* word , 
    const size_t index 
) {
    table -> num_items++ ; 
    size_t key = hash( table , word ) ; 
    if( (table -> buckets[key]).next_item ) {
        struct bucket_item* curr_node = (table -> buckets[key]).next_item ; 
        while( curr_node -> next_item ) {
            curr_node = curr_node -> next_item ; 
        }
        struct bucket_item* new_item = (struct bucket_item*) malloc( sizeof( struct bucket_item ) ) ;
        new_item -> word = malloc( sizeof( char ) * strlen( word ) ) ; 
        strcpy( new_item -> word , word ) ;
        new_item -> index = index ; 
        new_item -> next_item = NULL ; 
        curr_node -> next_item = new_item ; 
    }
    else {
        struct bucket_item* new_item = (struct bucket_item*) malloc( sizeof( struct bucket_item ) ) ;
        new_item -> word = malloc( sizeof( char ) * strlen( word ) ) ; 
        strcpy( new_item -> word , word ) ;
        new_item -> index = index ; 
        new_item -> next_item = NULL ; 
        ( (table -> buckets) + key ) -> next_item = new_item ;
    }
}

int hashtable_get( 
    struct hashtable* table , 
    const char* word 
) {
    size_t key = hash( table , word ) ;
    if( (table -> buckets[key]).next_item ) {
        struct bucket_item* curr_node = (table -> buckets[key]).next_item ; 
        while( curr_node )  {
            if( strcmp( curr_node -> word , word ) == 0 ) {
                return curr_node -> index ; 
            }
            curr_node = curr_node -> next_item ; 
        }
        return -1;
    }
    else {
        return -1;
    }
}
