#include "glove.h"

int main( int argc , char** argv ) {
    
    glove* instance = glove_create( 
        "../glove.6B.50d.txt" , 
        400000 , 
        50
    ) ; 

    float* embedding = glove_get_embedding( instance , "hello" ) ;
    for( int i = 0 ; i < 50 ; i++ ) {
        printf( "%f " , embedding[i] ) ;
    }
    printf( "\n" ) ;

    while( 1 ) {
        float score ;
        char word1[80]; 
        char word2[80]; 
        scanf( "%s %s" , word1 , word2 ) ; 
        if( glove_compare_cosine( instance , word1 , word2 , &score ) ) {
            printf( "Score: %f \n" , score ) ;
        } 
        else {
            printf( "Embeddings not found for word(s)\n" ) ; 
        }
    }
    
    glove_release( instance ) ; 

    return 0;
}
