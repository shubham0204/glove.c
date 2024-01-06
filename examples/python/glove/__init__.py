from .impl import glove_create , glove_release , glove_get_embedding 
from ctypes import c_float , POINTER , cast 

class GloVe:

    def __init__(
        self , 
        vectors_filepath: str , 
        vocab_size: int , 
        embedding_dims: int 
    ):
        self.instance = glove_create( vectors_filepath , vocab_size , embedding_dims )
        self.embedding_dims = embedding_dims

    def get_embedding(
        self , 
        word: str
    ) -> list[float]:
        embedding = glove_get_embedding( self.instance , word )
        embedding = cast( embedding , POINTER( c_float * self.embedding_dims ) )
        if embedding:
            return list( map( float , embedding.contents ) )
        else:
            return list( [ 0.0 for _ in range( self.embedding_dims ) ] )

    def __del__( self ):
        glove_release( self.instance )
