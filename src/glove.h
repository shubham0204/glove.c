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

glove* glove_create(
    const char* filename , 
    const __uint32_t vocab_size , 
    const __uint32_t embedding_dims
) ; 

float* glove_get_embedding(
    glove* instance , 
    const char* word 
) ;

void glove_release(
    glove* instance
) ;

int glove_compare_cosine(
    glove* instance , 
    const char* word1 , 
    const char* word2 , 
    float* score 
) 

int glove_compare_l2norm(
    glove* instance , 
    const char* word1 , 
    const char* word2 , 
    float* score 
)