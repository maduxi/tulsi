from random import randrange
import itertools
import time


for _ in itertools.repeat(None, 10):
    print(randrange(30)+5)
    time.sleep(5)