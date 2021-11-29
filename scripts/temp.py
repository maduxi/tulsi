import RPi.GPIO as GPIO
import time
import itertools
from w1thermsensor import W1ThermSensor, Unit
from w1thermsensor.errors import W1ThermSensorError
import logging
from aws_iot_message import send_message

logging.basicConfig(filename='/var/log/tulsi.log',
                    format='%(asctime)s - %(levelname)s - %(message)s',
                    level=logging.INFO)

file_name = '/tmp/tmp.txt'

client = "rpib"

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
    if i > 0:
        logging.info(f"Got {5 - i} false reads")
    else:
        logging.error(f"Got too many false reads")
    return temp

def write_file(temp):
    temp_str = str(round(temp, 2)).zfill(5)
    with open(file_name, "w") as my_file:
        my_file.write(temp_str)

temperature = get_temp()
status = 0
if temperature <= 0 or temperature > 40:
    logging.warning("Something went wrong.")
    status = -1
    write_file(33.33)
elif temperature < 18:
    logging.info(f"Start heat: Temperature: {temperature}.")
    GPIO.output(RELAIS_1_GPIO, GPIO.LOW)  # out
    status = 1
    write_file(temperature)
else:
    logging.info(f"Stop heat: Temperature: {temperature}.")
    GPIO.output(RELAIS_1_GPIO, GPIO.HIGH)  # on
    status = 0
    write_file(temperature)
send_message(
    message={
        't': temperature,
        's': status,
        'c': client
    },
    client_id=client,
    topic="tulsi/status"
)
