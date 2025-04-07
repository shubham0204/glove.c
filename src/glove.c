#include "glove.h"
#include "hashmap.c"
#include <stddef.h>

/**
 * Allocates memory for embeddings and words and stores them
 * in a `hashtable`
 */
glove*
glove_create(const char* filename, const size_t vocab_size, const size_t embedding_dims) {
    FILE*   file_ptr   = fopen(filename, "r");
    char**  words      = (char**)malloc(sizeof(char*) * vocab_size);
    float** embeddings = (float**)malloc(sizeof(float*) * vocab_size);

    // Assuming every word in the vocab
    // fits in the buffer completely
    char*  word_buffer = calloc(80, sizeof(char));
    size_t cntr        = 0;

    // Read embedding and word from the text file
    // and store them in `words` and `embeddings`
    while (fscanf(file_ptr, "%s", &word_buffer[0]) == 1 && cntr < vocab_size) {
        float* embedding = (float*)malloc(sizeof(float) * embedding_dims);
        for (size_t i = 0; i < embedding_dims; i++) {
            fscanf(file_ptr, "%f", (embedding + i));
        }
        char* word = (char*)malloc(sizeof(char) * strlen(word_buffer) + 1);
        memcpy(word, word_buffer, strlen(word_buffer) + 1);
        *(words + cntr)      = word;
        *(embeddings + cntr) = embedding;
        cntr++;
    }
    free(word_buffer);
    fclose(file_ptr);

    glove* vectors          = (glove*)malloc(sizeof(glove));
    vectors->str_word_size  = 80;
    vectors->vocab_size     = vocab_size;
    vectors->embedding_dims = embedding_dims;
    vectors->words          = words;
    vectors->embeddings     = embeddings;

    // Create a new hashtable and insert the words in it
    // If the permissible vocab size is smaller than 1,
    // set num_buckets = 5
    if (vocab_size * 0.95 < 5) {
        vectors->table = hashtable_create(5);
    } else {
        vectors->table = hashtable_create(vocab_size * 0.95);
    }

    for (size_t i = 0; i < vocab_size; i++) {
        hashtable_insert(vectors->table, vectors->words[i], i);
    }

    return vectors;
}

/**
 * Computes the key of the given `word` using the hash function
 * and searches for the embedding in the table
 */
float*
glove_get_embedding(glove* instance, const char* word) {
    int index = hashtable_get(instance->table, word);
    if (index >= 0) {
        return instance->embeddings[index];
    } else {
        return 0;
    }
}

/**
 * Retrieves the embeddings of `word1` and `word2`
 * and computes cosine similarity
 */
int
glove_compare_cosine(glove* instance, const char* word1, const char* word2, float* score) {
    float* embedding1 = glove_get_embedding(instance, word1);
    float* embedding2 = glove_get_embedding(instance, word2);
    if (embedding1 && embedding2) {
        float mag1        = 0.0f;
        float mag2        = 0.0f;
        float dot_product = 0.0f;
        for (size_t i = 0; i < instance->embedding_dims; i++) {
            dot_product += embedding1[i] * embedding2[i];
            mag1 += embedding1[i] * embedding1[i];
            mag2 += embedding2[i] * embedding2[i];
        }
        *score = dot_product / (sqrt(mag1) * sqrt(mag2));
        return 1;
    } else {
        return 0;
    }
}

/**
 * Retrives the embeddings of `word1` and `word2`
 * and compute the L2-norm of their difference
 */
int
glove_compare_l2norm(glove* instance, const char* word1, const char* word2, float* score) {
    float* embedding1 = glove_get_embedding(instance, word1);
    float* embedding2 = glove_get_embedding(instance, word2);
    if (embedding1 && embedding2) {
        float diffs_squared = 0.0f;
        for (size_t i = 0; i < instance->embedding_dims; i++) {
            diffs_squared += powf(embedding1[i] - embedding2[i], 2.0f);
        }
        *score = sqrt(diffs_squared);
        return 1;
    } else {
        return 0;
    }
}

/**
 * Releases the memory acquired by the embeddings
 * and the hashtable
 */
void
glove_release(glove* instance) {
    for (size_t i = 0; i < instance->vocab_size; i++) {
        free(instance->embeddings[i]);
        free(instance->words[i]);
    }
    hashtable_release(instance->table);
    free(instance->embeddings);
    free(instance->words);
    free(instance);
}
