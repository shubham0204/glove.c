#include "glove.h"

int main( int argc , char** argv ) {
    
    glove* instance = glove_create( 
        "glove.6B.50d.txt" , 
        400000 , 
        50
    ) ; 

    while( 1 ) {
        float score ;
        if( glove_compare_cosine( instance , argv[1] , argv[2] , &score ) ) {
            printf( "Score: %f \n" , score ) ;
        } 
        else {
            printf( "Embeddings not found for word(s)\n" ) ; 
        }
    }
    
    glove_release( instance ) ; 

    return 0;
}
