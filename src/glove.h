/**
 * @file glove.h
 * @author Shubham Panchal
 * @brief Declares functions for `glove.c`
 * @version 0.1
 * @date 2023-12-18
 * @copyright Shubham Panchal (c) 2023
 */

#ifndef GLOVE_H
#define GLOVE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Holds pointers to words and embeddings
 */
typedef struct {
    size_t            vocab_size;
    size_t            embedding_dims;
    size_t            str_word_size;
    char**            words;
    float**           embeddings;
    struct hashtable* table;
} glove;

/**
 * @brief Creates a new instance of `glove`
 * @param filename Path to `vectors.txt` or the text file containing the
 * vectors according to `GloVe`'s format
 * @param vocab_size Number of words in the text file
 * @param embedding_dims Number of elements in each embedding OR dimensionality
 * of the embedding
 * @return glove* An instance of glove that can be used to access word
 * embeddings
 */
glove* glove_create(const char* filename, const size_t vocab_size, const size_t embedding_dims);

/**
 * @brief Returns the embedding for the given `word`
 * @param instance The `glove*` instance to be used
 * @param word Word for which embedding will be returned
 * @return float* Embedding for the given `word`. Equals `0` if the embedding
 * was not found.
 */
float* glove_get_embedding(glove* instance, const char* word);

/**
 * @brief Compute the cosine similarity between the embeddings of words `word1`
 * and `word2`
 * @param instance The `glove*` instance to be used
 * @param word1 1st word to compare
 * @param word2 2nd word to compare
 * @param score cosine similarity between the embeddings of words `word1` and
 * `word2`
 * @return int 1 if the score was written successfully, else 0 (one of `word1`
 * or `word2` do not have embedddings)
 */
int glove_compare_cosine(glove* instance, const char* word1, const char* word2, float* score);

/**
 * @brief Compute the L2 norm of the difference between the embeddings of words
 * `word1` and `word2`
 * @param instance The `glove*` instance to be used
 * @param word1 1st word to compare
 * @param word2 2nd word to compare
 * @param score L2 norm of the difference between the embeddings of `word1` and
 * `word2`
 * @return int int 1 if the score was written successfully, else 0 (one of
 * `word1` or `word2` do not have embedddings)
 */
int glove_compare_l2norm(glove* instance, const char* word1, const char* word2, float* score);

/**
 * @brief Release the memory allocated to the `glove` instance
 * @param instance `glove*` instance created with `glove_create` function
 */
void glove_release(glove* instance);

#endif
