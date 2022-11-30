#!/usr/bin/python3

import time
from threading import Thread

import serial
import pyautogui


class MouseMover:

    def __init__(self, speed=6, update_interval=0.002):
        self._thread = None
        self._do_stop_thread = False
        self._x_d = 0
        self._y_d = 0
        self._dd = 0.1
        self._max_speed = speed
        self._acceleration = 0.005
        self._update_interval = update_interval
        pass

    def stop_x(self):
        self._x_d = 0

    def stop_y(self):
        self._y_d = 0

    def move_left(self):
        self._x_d = -self._max_speed
        self._dd = 0.1

    def move_right(self):
        self._x_d = self._max_speed
        self._dd = 0.1

    def move_up(self):
        self._y_d = -self._max_speed
        self._dd = 0.1

    def move_down(self):
        self._y_d = self._max_speed
        self._dd = 0.1

    def run(self):
        self._do_stop_thread = False
        self._thread = Thread(target=self._threadLoop)
        self._thread.start()

    def stop(self):
        self.stop_x()
        self.stop_y()
        self._do_stop_thread = True
        self._thtread.join()

    def _threadLoop(self):
        while not self._do_stop_thread:
            if self._x_d != 0 or self._y_d != 0:
                ## compute actual axis speed with acceleration
                if self._dd < 1.0:
                    self._dd += self._acceleration
                move_x = self._x_d * self._dd
                # clamp
                if self._x_d != 0:
                    if move_x > -1 and move_x <= 0:
                        move_x = -1
                    elif move_x < 1 and move_x >= 0:
                        move_x = 1
                move_y = self._y_d * self._dd
                # clamp
                if self._y_d != 0:
                    if move_y > -1 and move_y <= 0:
                        move_y = -1
                    elif move_y < 1 and move_y >= 0:
                        move_y = 1
                # move mouse
                pyautogui.move(move_x, move_y, _pause=False)
            # also scale the sleep with the accerlation
            time.sleep(self._update_interval / self._dd)


def main():
    serial_device_path = "/dev/ttyACM0"
    mouse_mover = MouseMover()
    mouse_mover.run()

    while True:
        serial_con = serial.Serial(serial_device_path, baudrate=9600)
        while serial_con.isOpen():
            if True:
                line = str(serial_con.readline())
                print(line)
                if "LEFT_LOW" in line or "RIGHT_LOW" in line:
                    mouse_mover.stop_x()
                if "LEFT_HIGH" in line:
                    mouse_mover.move_left()
                if "RIGHT_HIGH" in line:
                    mouse_mover.move_right()
                if "FRONT_LOW" in line or "BACK_LOW" in line:
                    mouse_mover.stop_y()
                if "FRONT_HIGH" in line:
                    mouse_mover.move_up()
                if "BACK_HIGH" in line:
                    mouse_mover.move_down()
                if "UP_HIGH" in line:
                    pyautogui.click()
                if "DOWN_HIGH" in line:
                    pyautogui.click()
        time.sleep(2.0)
    

if __name__ == "__main__":
    main()
