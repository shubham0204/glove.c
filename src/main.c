#include <sys/time.h>
#include <unistd.h>
#include "glove.c"

int main( int argc , char** argv ) {

    char** words ; 
    float** embeddings ; 

    glove* instance = glove_create( 
        "glove.6B.50d.txt" , 
        400000 , 
        50
    ) ; 

    long start, end;
    struct timeval timecheck;
    gettimeofday(&timecheck, NULL);
    start = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;
    gettimeofday(&timecheck, NULL);
    end = (long)timecheck.tv_sec * 1000 + (long)timecheck.tv_usec / 1000;

    float* embedding = glove_get_embedding( instance , argv[1] ) ;

    printf("%ld milliseconds elapsed\n", (end - start));

    if( embedding ) {
        for( int i = 0 ; i < instance -> embedding_dims ; i++ ) {
            printf( "%f " , embedding[i] ) ;
        }
        printf( "\n" ) ; 
    }
    else {
        printf( "embedding not found" ) ;
    }
    

    glove_release( instance ) ; 

    return 0;
}