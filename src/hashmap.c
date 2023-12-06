#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EXPAND_LOADFACTOR_THRESHOLD 0.75
#define EXPAND_SCALE 2.0
#define SHRINK_LOADFACTOR_THRESHOLD 0.2
#define INITIAL_NUM_BUCKETS 5

struct bucket_item {
    char* word ; 
    __uint32_t index ; 
    struct bucket_item* next_item ; 
} ; 

struct hashtable {
    struct bucket_item* buckets ; 
    __uint32_t num_buckets ;  
    __uint32_t num_items ; 
} hashtable ; 

struct hashtable* hashtable_create() {
    struct hashtable* table = (struct hashtable*) malloc( sizeof( hashtable ) ) ;
    table -> buckets = (struct bucket_item*) calloc( INITIAL_NUM_BUCKETS , sizeof( struct bucket_item ) ) ; 
    table -> num_buckets = INITIAL_NUM_BUCKETS ;
    table -> num_items = 0 ; 
    return table ; 
}

__uint32_t hash( 
    struct hashtable* table , 
    char* word 
) {
    __uint32_t h = 0 ; 
    for( int i = 0 ; i < strlen( word ) ; i++ ) {
        h = h * 31 + (__uint32_t)word[ i ] ;
    }
    return h % table -> num_buckets ;  
}

void hashtable_print( 
    struct hashtable* table 
) {
    for( int i = 0 ; i < table -> num_buckets ; i++ ) {
        struct bucket_item* curr_node = (table -> buckets[i]).next_item ; 
        printf( "%i " , i ) ; 
        while( curr_node ) {
            printf( "%s " , curr_node -> word ) ; 
            curr_node = curr_node -> next_item ; 
        }
        printf( "\n" ) ; 
    }
}

void hashtable_release_nodes( struct hashtable* table ) {
     for( int i = 0 ; i < table -> num_buckets ; i++ ) {
        struct bucket_item* curr_node = (table -> buckets[i]).next_item ; 
        while( curr_node ) {
            struct bucket_item* next_item = curr_node -> next_item ; 
            free( curr_node -> next_item ) ; 
            curr_node = next_item ; 
        }
    }
    free( table -> buckets ) ; 
}

void hashtable_release( struct hashtable* table ) {
    for( int i = 0 ; i < table -> num_buckets ; i++ ) {
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
    char* word , 
    __uint32_t index 
) {
    table -> num_items++ ; 
    float load_factor = (float)table -> num_items / (float)table -> num_buckets ; 
    printf( "%f \n" , load_factor ) ; 
    if( load_factor > EXPAND_LOADFACTOR_THRESHOLD )  {
        __uint32_t new_hashtable_size = table -> num_buckets * EXPAND_SCALE ; 
        printf( "Resized table size %i .............................\n" , new_hashtable_size ) ; 
        struct hashtable* new_table = (struct hashtable*) malloc( sizeof( struct hashtable ) ) ;
        new_table -> buckets = (struct bucket_item*) malloc( sizeof( struct bucket_item ) * new_hashtable_size ) ; 
        new_table -> num_buckets = new_hashtable_size ;
        for( int i = 0 ; i < table -> num_buckets ; i++ ) {
            struct bucket_item* table_curr_node = (table -> buckets[i]).next_item ; 
            while( table_curr_node ) {
                __uint32_t key = hash( new_table , table_curr_node -> word ) ; 
                if( (new_table -> buckets) + key ) {
                    struct bucket_item* curr_node = (new_table -> buckets) + key ; 
                    while( curr_node -> next_item ) {
                        curr_node = curr_node -> next_item ; 
                    }
                    struct bucket_item* new_item = (struct bucket_item*) malloc( sizeof( struct bucket_item ) ) ;
                    new_item -> word = malloc( sizeof( char ) * strlen( table_curr_node -> word ) ) ; 
                    strcpy( new_item -> word , table_curr_node -> word ) ;
                    new_item -> index = table_curr_node -> index ; 
                    new_item -> next_item = NULL ; 
                    curr_node -> next_item = new_item ; 
                }
                else {
                    struct bucket_item* new_item = (struct bucket_item*) malloc( sizeof( struct bucket_item ) ) ;
                    new_item -> word = malloc( sizeof( char ) * strlen( table_curr_node -> word ) ) ; 
                    strcpy( new_item -> word , table_curr_node -> word ) ;
                    new_item -> index = table_curr_node -> index ; 
                    new_item -> next_item = NULL ; 
                    *( (new_table -> buckets) + key ) = *new_item ;
                }
                table_curr_node = table_curr_node -> next_item ; 
            }
        }
        hashtable_release_nodes( table ) ;
        table -> buckets = new_table -> buckets ;
        table -> num_buckets = new_table -> num_buckets ; 
        free( new_table ) ; 
    }
   
    __uint32_t key = hash( table , word ) ; 
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

__uint32_t hashtable_get( struct hashtable* table , char* word ) {
    __uint32_t key = hash( table , word ) ;
    if( (table -> buckets[key]).next_item ) {
        struct bucket_item* curr_node = (table -> buckets[key]).next_item ; 
        while( curr_node )  {
            if( strcmp( curr_node -> word , word ) == 0 ) {
                return curr_node -> index ; 
            }
            curr_node = curr_node -> next_item ; 
        }
    }
    else {
        return 0;
    }
}




void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}


int main( int argc , char** argv ) {
    srand((unsigned int)(time(NULL)));

    struct hashtable* table = hashtable_create() ; 

    char value[10] ; 
    for( int i = 0 ; i < 10 ; i++ ) {
        rand_str( value , 10 ) ; 
        printf("--------------------------------- \n %s\n " , value ) ; 
        hashtable_insert( table , value , 1 ) ;
        hashtable_print( table ) ; 
        // printf( "%i\n" , hashtable_get( table , value ) ) ;
    }

    //hashtable_print( table ) ; 
    


    return 0;
}