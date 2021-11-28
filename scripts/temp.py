import RPi.GPIO as GPIO
import time
import itertools
from w1thermsensor import W1ThermSensor, Unit
from w1thermsensor.errors import W1ThermSensorError
import logging

logging.basicConfig(filename='/var/log/tulsi.log', format='%(asctime)s - %(levelname)s - %(message)s', level=logging.INFO)

GPIO.setmode(GPIO.BCM)  # GPIO Numbers instead of board numbers
GPIO.setwarnings(False)

RELAIS_1_GPIO = 25
GPIO.setup(RELAIS_1_GPIO, GPIO.OUT)  # GPIO Assign mode

sensor = W1ThermSensor()


def get_temp():
    temp = 0
    i = 5
    while i > 0 and temp == 0:
        try:
            temp = sensor.get_temperature(Unit.DEGREES_C)
        except W1ThermSensorError:
            i -= 1
            time.sleep(1)
    if i>0:
        logging.info(f"Got {5-i} false reads")
    else:
        logging.error(f"Got too many false reads")
    return temp


temperature = get_temp()
if temperature == 0:
    logging.warning("Something went wrong")
elif temperature < 18:
    logging.info(f"Start heat: Temperature: {temperature}.")
    GPIO.output(RELAIS_1_GPIO, GPIO.LOW)  # out
else:
    logging.info(f"Stop heat: Temperature: {temperature}.")
    GPIO.output(RELAIS_1_GPIO, GPIO.HIGH)  # on
