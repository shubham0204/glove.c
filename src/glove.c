#include "glove.h"
#include "hashmap.c"
#include <math.h>

typedef struct {
    __uint32_t vocab_size ; 
    __uint32_t embedding_dims ; 
    __uint32_t str_word_size ; 
    char** words ; 
    float** embeddings ; 
    struct hashtable* table ;
} glove ; 

extern glove* glove_create(
    const char* filename , 
    const __uint32_t vocab_size , 
    const __uint32_t embedding_dims
) {
    FILE* file_ptr = fopen( filename , "r" ) ; 
    char** words = (char**) malloc( sizeof( char* ) * vocab_size ) ; 
    float** embeddings = (float**) malloc( sizeof( float* ) * vocab_size ) ; 
    
    long cntr = 0 ;
    char word_buffer[ 80 ] ; 
    while( fscanf( file_ptr , "%s" , word_buffer ) == 1 ) {
        float* embedding = (float*) malloc( sizeof( float ) * embedding_dims ) ; 
        for( int i = 0 ; i < embedding_dims ; i++ ) {
            fscanf( file_ptr , "%f" , (embedding + i) ) ; 
        }
        char* word = (char*) malloc( sizeof( char ) * strlen( word_buffer ) ) ;
        strncpy( word , word_buffer , strlen( word_buffer ) ) ; 
        *( words + cntr ) = word ; 
        *( embeddings + cntr ) = embedding ; 
        cntr++ ; 
    }
    fclose( file_ptr ) ; 
    glove* vectors = (glove*) malloc( sizeof( glove ) ) ; 
    vectors -> str_word_size = 80 ; 
    vectors -> vocab_size = vocab_size ; 
    vectors -> embedding_dims = embedding_dims ; 
    vectors -> words = words ; 
    vectors -> embeddings = embeddings ; 
    vectors -> table = hashtable_create( vocab_size * 0.95 ) ; 
    for( __uint64_t i = 0 ; i < vocab_size ; i++ ) {
        hashtable_insert( vectors -> table ,  vectors -> words[i] , i ) ;    
    }
    return vectors; 
}

extern float* glove_get_embedding(
    glove* instance , 
    const char* word 
) {
    int index = hashtable_get( instance -> table , word ) ; 
    if( index ) {
        return instance -> embeddings[ index ] ; 
    }
    else {
        return 0 ;
    }
}

extern int glove_compare_cosine(
    glove* instance , 
    const char* word1 , 
    const char* word2 , 
    float* score 
) {
    float* embedding1 = glove_get_embedding( instance , word1 ) ; 
    float* embedding2 = glove_get_embedding( instance , word2 ) ; 
    if( embedding1 && embedding2 ) {
        float mag1 = 0.0f ; 
        float mag2 = 0.0f ; 
        float dot_product = 0.0f ; 
        for( int i = 0 ; i < instance -> embedding_dims ; i++ ) {
            dot_product += embedding1[i] * embedding2[i] ; 
            mag1 += embedding1[i] * embedding1[i] ; 
            mag2 += embedding2[i] * embedding2[i] ; 
        }
        *score = dot_product / ( sqrt( mag1 ) * sqrt( mag2 ) ) ;
        return 1 ; 
    }
    else {
        return 0 ; 
    }
}

extern int glove_compare_l2norm(
    glove* instance , 
    const char* word1 , 
    const char* word2 , 
    float* score 
) {
    float* embedding1 = glove_get_embedding( instance , word1 ) ; 
    float* embedding2 = glove_get_embedding( instance , word2 ) ; 
    if( embedding1 && embedding2 ) {
        float diffs_squared = 0.0f ; 
        for( int i = 0 ; i < instance -> embedding_dims ; i++ ) {
            diffs_squared += powf( embedding1[i] - embedding2[i] , 2.0f ) ; 
        }
        *score = sqrt( diffs_squared ) ; 
        return 1 ; 
    }
    else {
        return 0 ; 
    }
}

extern void glove_release(
    glove* instance
) {
    for( int i = 0 ; i < instance -> vocab_size ; i++ ) {
        free( instance -> embeddings[i] ) ; 
        free( instance -> words[i] ) ;
    }
    hashtable_release( instance -> table ) ; 
    free( instance -> embeddings ) ; 
    free( instance -> words ) ; 
    free( instance ) ; 
}