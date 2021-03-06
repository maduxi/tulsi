import time

import RPi.GPIO as GPIO

from temp import file_name

GPIO.setmode(GPIO.BCM)

d1 = 14
d2 = 15
d3 = 18
d4 = 23
dp = 7
a = 27
b = 10
c = 8
d = 11
e = 9
f = 22
g = 24

segments = (a, b, c, d, e, f, g, dp)
digits = (d1, d2, d3, d4)

for segment in segments:
    GPIO.setup(segment, GPIO.OUT)
    GPIO.output(segment, 1)

for digit in digits:
    GPIO.setup(digit, GPIO.OUT)
    GPIO.output(digit, 0)

num = {' ': (0, 0, 0, 0, 0, 0, 0),
       '0': (1, 1, 1, 1, 1, 1, 0),
       '1': (0, 1, 1, 0, 0, 0, 0),
       '2': (1, 1, 0, 1, 1, 0, 1),
       '3': (1, 1, 1, 1, 0, 0, 1),
       '4': (0, 1, 1, 0, 0, 1, 1),
       '5': (1, 0, 1, 1, 0, 1, 1),
       '6': (1, 0, 1, 1, 1, 1, 1),
       '7': (1, 1, 1, 0, 0, 0, 0),
       '8': (1, 1, 1, 1, 1, 1, 1),
       '9': (1, 1, 1, 1, 0, 1, 1)}


def read_temp():
    with open(file_name) as f:
        first_line = f.readline()
    return first_line

try:
    while True:
        n = read_temp()
        s = str(n).rjust(4)[::-1]
        # print(s)
        for digit in range(4):
            for loop in range(0, 7):
                GPIO.output(segments[loop], num[s[digit]][loop])
                if (digit == 2):
                    GPIO.output(dp, 1)
                else:
                    GPIO.output(dp, 0)
            GPIO.output(digits[digit], 0)
            time.sleep(0.001)
            GPIO.output(digits[digit], 1)
finally:
    GPIO.cleanup()


