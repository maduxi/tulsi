import RPi.GPIO as GPIO
from w1thermsensor import W1ThermSensor, Unit
from w1thermsensor.errors import W1ThermSensorError

GPIO.setmode(GPIO.BCM)  # GPIO Numbers instead of board numbers

RELAIS_1_GPIO = 25
GPIO.setup(RELAIS_1_GPIO, GPIO.OUT)  # GPIO Assign mode

sensor = W1ThermSensor()


def get_temp():
    temp = 20
    try:
        temp = sensor.get_temperature(Unit.DEGREES_C)
    except W1ThermSensorError:
        # handle ValueError exception
        pass
    return temp


temperature = get_temp()
print(f"Temperature: {temperature}")
if temperature<22:
    print("start heat")
    GPIO.output(RELAIS_1_GPIO, GPIO.LOW)  # out
else:
    print("stop heat")
    GPIO.output(RELAIS_1_GPIO, GPIO.HIGH)  # on
