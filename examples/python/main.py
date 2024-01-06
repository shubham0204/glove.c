from glove import GloVe
import time

glove = GloVe( "glove.6B.50d.txt" , 400000 , 50 )

t1 = time.time_ns()
vec = glove.get_embedding( "hello" )
print( vec )
print( time.time_ns() - t1 )