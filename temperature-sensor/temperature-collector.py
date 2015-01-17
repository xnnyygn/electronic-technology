import serial
import RPi.GPIO as GPIO
import datetime
from time import sleep
import httplib

PIN_LED_PASSED=22
PIN_LED_FAILED=23

API_KEY='API_KEY_HERE'
HOST='api.yeelink.net'
URI='/v1.0/device/15530/sensor/26740/datapoints'

def loop(s):
    s.writelines(['G']) # send serial command to get current temperature
    c = s.readline().strip()
    if not c:
        return
    now = datetime.datetime.now().isoformat()
    print '[INFO]', now, c
    if send_sensor_data(now, c):
        GPIO.output(PIN_LED_PASSED, GPIO.HIGH)
    else:
        GPIO.output(PIN_LED_FAILED, GPIO.HIGH)
    sleep(1)
    GPIO.output(PIN_LED_PASSED, GPIO.LOW)
    GPIO.output(PIN_LED_FAILED, GPIO.LOW)
    sleep(10) # sleep 10 seconds for yeelink limit(> 10s after previous request)

def send_sensor_data(timestamp, value):
    payload = '{"timestamp":"' + timestamp + '","value":' + value + '}'
    headers = {'U-ApiKey': API_KEY}

    conn = httplib.HTTPConnection(HOST)
    conn.request('POST', URI, payload, headers)
    resp = conn.getresponse()
    conn.close()
    if resp.status == 200:
        return True
    else:
        print '[WARN] failed to send sensor data,', resp.reason
        return False

if __name__ == '__main__':
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(PIN_LED_PASSED, GPIO.OUT)
    GPIO.setup(PIN_LED_FAILED, GPIO.OUT)

    s = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)
    s.open()
    print '[INFO] sensor data collector start'
    try:
        while True:
            loop(s)
    except KeyboardInterrupt:
        s.close()
        GPIO.cleanup()
