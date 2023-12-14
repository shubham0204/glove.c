#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
) ; 

extern float* glove_get_embedding(
    glove* instance , 
    char* word 
) ;

extern void glove_release(
    glove* instance
) ;