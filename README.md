# `glove.c` Port of GloVe Embeddings in C

## Usage

The program below shows the usage of the `glove.c` with pretrained embeddings taken from [StanfordNLP/GloVe](https://github.com/stanfordnlp/GloVe?tab=readme-ov-file#download-pre-trained-word-vectors) repository. The embeddings are derived from the Wikipedia 2014 + Gigaword 5 datasets consisting of 6B tokens and 400K vocab with 50 dimensions.

### Usage in C

> See [src/main.c](https://github.com/shubham0204/glove.c/blob/main/src/main.c)

```c
#include "glove.h"

int main( int argc , char** argv ) {
    glove* instance = glove_create( 
        "glove.6B.50d.txt" , /* vectors */
        400000 ,             /* vocab size */
        50                   /* vector size */
    ) ; 
    float* embedding = glove_get_embedding( instance , argv[1] ) ;
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
```

Compile the program with `glove.c` and `libmath`,

```
$> gcc main.c glove.c -o main -lm
$> ./main hello
```

### Usage in Java

> See [examples/java](https://github.com/shubham0204/glove.c/tree/main/examples/java)

```java
import java.util.Arrays;

public class Main {

    public static void main(String[] args) {
        String word = "hello" ; 
        GloVe glove = new GloVe( 
            "glove.6B.50d.txt" , 
            400000 , 
            50
        ) ; 
        float[] embedding = glove.getEmbedding( word ) ;
        System.out.println( "Embedding: " + Arrays.toString( embedding ) ) ;
    }
}
```

### Usage in Python

> See [examples/python](https://github.com/shubham0204/glove.c/tree/main/examples/python)

```python
from glove import GloVe
import time

glove = GloVe( "glove.6B.50d.txt" , 400000 , 50 )

vec = glove.get_embedding( "hello" )
print( vec )
```

## Working

`glove.c` uses a hashtable with open-chaining to get near-constant access times for all embeddings, at the expense of extra storage overhead.


## Using GloVe Trained On A Custom Corpus

The steps for training a GloVe model on a custom corpus is provided on the [official GitHub repository](https://github.com/stanfordnlp/GloVe/tree/master/src#readme). Once the training is started with the by executing the `demo.sh` script, we see the following in output written on the console,

```
...
TRAINING MODEL
Read 60666468 lines.
Initializing parameters...Using random seed 1702550061
done.
vector size: 50
vocab size: 71290
x_max: 10.000000
alpha: 0.750000
12/14/23 - 10:35.36AM, iter: 001, cost: 0.071237
...
12/14/23 - 10:52.36AM, iter: 014, cost: 0.036445
12/14/23 - 10:53.54AM, iter: 015, cost: 0.036244
$ python eval/python/evaluate.py
...
```

After training is complete, the `vectors.txt` file can be found in the root directory of the project. Along with `vectors.txt`, we also need `vector size` and `vocab size` from console output, as given above. These three parameters would go into the `glove_create` function which returns an instance of `glove` and allows us to get embeddings for words. 
