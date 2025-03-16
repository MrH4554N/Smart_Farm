from ble import *
from yolobit import *
button_a.on_pressed = None
button_b.on_pressed = None
button_a.on_pressed_ab = button_b.on_pressed_ab = -1
from mqtt import *
from event_manager import *
import time
from aiot_lcd1602 import LCD1602
from machine import Pin, SoftI2C
from plantbit2_dht20 import DHT20

def on_ble_message_string_receive_callback(chu_E1_BB_97i):
  global Serial_ID, Serial_Device, Serial_Data, cmd, SurfaceMoist, Light, NodeID, Central_Node
  cmd = chu_E1_BB_97i

ble.on_receive_msg("string", on_ble_message_string_receive_callback)

event_manager.reset()

def on_event_timer_callback_Q_b_L_d_N():
  global chu_E1_BB_97i, Serial_ID, Serial_Device, Serial_Data, cmd, SurfaceMoist, Light, NodeID, Central_Node
  if cmd == '1':
    pin14.write_analog(round(translate(0, 0, 100, 0, 1023)))
  elif cmd == '2':
    pin14.write_analog(round(translate(70, 0, 100, 0, 1023)))
  elif cmd == '3':
    pin4.servo_write(90)
  elif cmd == '4':
    pin4.servo_write(10)

event_manager.add_timer_event(100, on_event_timer_callback_Q_b_L_d_N)

def on_event_timer_callback_j_L_o_J_X():
  global chu_E1_BB_97i, Serial_ID, Serial_Device, Serial_Data, cmd, SurfaceMoist, Light, NodeID, Central_Node
  SurfaceMoist = round(translate((pin2.read_analog()), 0, 4095, 0, 100))
  Light = round(translate((pin1.read_analog()), 0, 4095, 0, 100))
  if SurfaceMoist < '50':
    pin14.write_analog(round(translate(70, 0, 100, 0, 1023)))
    time.sleep_ms(60000)
    pin14.write_analog(round(translate(0, 0, 100, 0, 1023)))
  if Light < '5':
    pin4.servo_write(90)
    time.sleep_ms(60000)
    pin4.servo_write(0)

event_manager.add_timer_event(10000, on_event_timer_callback_j_L_o_J_X)

def on_ble_disconnected_callback():
  global chu_E1_BB_97i, Serial_ID, Serial_Device, Serial_Data, cmd, SurfaceMoist, Light, NodeID, Central_Node
  display.show(Image("11111:10001:10001:10001:11111"))
  while not ble.is_connected():
    ble.connect(Central_Node)
  pin14.write_analog(round(translate(0, 0, 100, 0, 1023)))
  pin4.servo_write(10)
  display.show(Image("00000:04440:04440:04440:00000"))

ble.on_disconnected(on_ble_disconnected_callback)

aiot_lcd1602 = LCD1602()

dht20 = DHT20()

def on_event_timer_callback_r_L_K_c_X():
  global chu_E1_BB_97i, Serial_ID, Serial_Device, Serial_Data, cmd, SurfaceMoist, Light, NodeID, Central_Node
  aiot_lcd1602.move_to(0, 0)
  aiot_lcd1602.putstr((''.join([str(x) for x in ['C', dht20.dht20_temperature(), 'A%', dht20.dht20_humidity(), 'S%', round(translate((pin2.read_analog()), 0, 4095, 0, 100)), 'L', round(translate((pin1.read_analog()), 0, 4095, 0, 100))]])))
  G_E1_BB_ADi_data_qua_bluetooth(NodeID, 'TEMP', dht20.dht20_temperature())
  G_E1_BB_ADi_data_qua_bluetooth(NodeID, 'AIRM', dht20.dht20_humidity())
  G_E1_BB_ADi_data_qua_bluetooth(NodeID, 'LIGHT', round(translate((pin1.read_analog()), 0, 4095, 0, 100)))
  G_E1_BB_ADi_data_qua_bluetooth(NodeID, 'SURFM', round(translate((pin2.read_analog()), 0, 4095, 0, 100)))

event_manager.add_timer_event(30000, on_event_timer_callback_r_L_K_c_X)

# Mô tả hàm này...
def G_E1_BB_ADi_data_qua_bluetooth(Serial_ID, Serial_Device, Serial_Data):
  global chu_E1_BB_97i, cmd, SurfaceMoist, Light, NodeID, Central_Node, aiot_lcd1602, dht20
  ble.send((''.join([str(x2) for x2 in ['!', Serial_ID, ':', Serial_Device, ':', Serial_Data, '#']])))

def on_ble_message_string_receive_callback(chu_E1_BB_97i):
  global Serial_ID, Serial_Device, Serial_Data, cmd, SurfaceMoist, Light, NodeID, Central_Node
  cmd = chu_E1_BB_97i

ble.on_receive_msg("string", on_ble_message_string_receive_callback)

if True:
  display.show(Image("33333:30003:30003:30003:33333"))
  mqtt.connect_wifi('ACLAB', 'ACLAB2023')
  NodeID = 'A'
  cmd = ''
  Central_Node = 'ohstem-yolobit-50ac'
  while not ble.is_connected():
    ble.connect(Central_Node)
  display.show(Image("00000:04440:04440:04440:00000"))

while True:
  event_manager.run()
  time.sleep_ms(1000)
  time.sleep_ms(10)
