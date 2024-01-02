#include "glove.h"
#include <jni.h>

JNIEXPORT jlong JNICALL Java_com_shubham0204_projects_glove_GloVe_create(
    JNIEnv* env , 
    jobject this_object , 
    jstring embeddings_filepath , 
    jint vocab_size , 
    jint embeddings_dim
) {
    jboolean is_copy ; 
    const char* filepath = (*env) -> GetStringUTFChars( env , embeddings_filepath , &is_copy ) ; 
    glove* instance = glove_create( filepath , vocab_size , embeddings_dim ) ; 
    (*env) -> ReleaseStringUTFChars( env , embeddings_filepath , filepath ) ; 
    return (jlong) instance ; 
}

JNIEXPORT jfloatArray JNICALL Java_com_shubham0204_projects_glove_GloVe_annotate(
    JNIEnv* env , 
    jobject this_object , 
    jlong instance_addr , 
    jstring word
) {
    jboolean is_copy ; 
    const char* input_word = (*env) -> GetStringUTFChars( env , word , &is_copy ) ; 
    glove* instance = (glove*) instance_addr ; 
    float* embedding = glove_get_embedding( instance , input_word ) ; 
    (*env) -> ReleaseStringUTFChars( env , word , input_word ) ; 
    jfloatArray output_embedding = (*env) -> NewFloatArray( env , instance -> embedding_dims ) ; 
    if( embedding ) {
        (*env) -> SetFloatArrayRegion( env , output_embedding , 0 , instance -> embedding_dims , embedding ) ; 
        return output_embedding ; 
    }
    else {
        return output_embedding ; 
    }
}

JNIEXPORT void JNICALL Java_com_shubham0204_projects_glove_GloVe_release(
    JNIEnv* env , 
    jobject this_object , 
    jlong instance_addr
) {
    glove* instance = (glove*) instance_addr ; 
    glove_release( instance ) ; 
}