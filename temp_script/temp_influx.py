from random import randrange
import itertools
import time
import requests

url = 'http://localhost:8086/api/v2/write?bucket=tulsi'
headers = {"Authorization": "Token tulsi:tulsi"}

for _ in itertools.repeat(None, 10):
    temp = randrange(30)+5
    print(temp)
    data = f'tulsi-temp value={temp}'
    x = requests.post(url, headers=headers, data=data)
    time.sleep(5)
