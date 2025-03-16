from yolobit import *
button_a.on_pressed = None
button_b.on_pressed = None
button_a.on_pressed_ab = button_b.on_pressed_ab = -1
from mqtt import *
from event_manager import *
import time
from ble import *

event_manager.reset()

def on_mqtt_message_receive_callback__pump_(th_C3_B4ng_tin):
  global chu_E1_BB_97i, NODE_ID, cmd
  ble.send(th_C3_B4ng_tin)

def on_mqtt_message_receive_callback__blink_led_(th_C3_B4ng_tin):
  global chu_E1_BB_97i, NODE_ID, cmd
  ble.send(th_C3_B4ng_tin)

def on_event_timer_callback_P_q_V_q_c():
  global chu_E1_BB_97i, th_C3_B4ng_tin, NODE_ID, cmd
  mqtt.on_receive_message('pump', on_mqtt_message_receive_callback__pump_)
  mqtt.on_receive_message('blink-led', on_mqtt_message_receive_callback__blink_led_)

event_manager.add_timer_event(500, on_event_timer_callback_P_q_V_q_c)

def on_ble_message_string_receive_callback(chu_E1_BB_97i):
  global th_C3_B4ng_tin, NODE_ID, cmd
  print(chu_E1_BB_97i, end =' ')

ble.on_receive_msg("string", on_ble_message_string_receive_callback)

def on_ble_message_string_receive_callback(chu_E1_BB_97i):
  global th_C3_B4ng_tin, NODE_ID, cmd
  print(chu_E1_BB_97i, end =' ')

ble.on_receive_msg("string", on_ble_message_string_receive_callback)

if True:
  display.show(Image("33333:30003:30003:30003:33333"))
  mqtt.connect_wifi('ACLAB', 'ACLAB2023')
  mqtt.connect_broker(server='io.adafruit.com', port=1883, username='H4554N', password='aio_Jeeh73BQOdVIau5ZHmbgNtaUxytG')
  NODE_ID = '0'
  cmd = ''
  pin14.write_analog(round(translate(0, 0, 100, 0, 1023)))
  pin4.servo_write(0)
  display.show(Image("33333:34443:34443:34443:33333"))

while True:
  event_manager.run()
  time.sleep_ms(1000)
  time.sleep_ms(10)
