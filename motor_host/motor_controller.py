#!/usr/bin/python3

import time
import argparse

import serial


class ClawMover:

    def __init__(self, serial_device_path, do_reconnect=True,
                 do_disconnect=False):
        self._serial_device_path = serial_device_path
        self._serial_con = None
        self._baudrate = 38400
        self._do_reconnect = do_reconnect
        self._do_disconnect = do_disconnect

    def connect(self):
        if not self._serial_con or not self._serial_con.isOpen():
            self._serial_con = \
                    serial.Serial(self._serial_device_path,
                                  self._baudrate)

    def reconnect(self):
        if self.is_connected():
            self._serial_con.close()
            self._serial_con = None
        self.connect()

    def is_connected(self):
        return (self._serial_con and self._serial_con.isOpen())

    def send_sequence_str(self, sequence_str, retries=5):
        if self._do_reconnect:
            self.reconnect()
        for i in range(retries):
            ascii_str = (sequence_str + '\n').encode('ascii')
            print("send cmd:", ascii_str)
            self._serial_con.writelines([ascii_str])
            rep = str(self._serial_con.readline())
            if not "ERROR" in rep and not "NOCMD" in rep:
                print("Send cmd OK.")
                break
            else:
                print("error sending cmd retry..")
        if self._do_disconnect:
            self._serial_con.close()

    def sequence_to_sequence_str(self, sequence):
        header = "PLAY;"
        footer = "YALP;"
        sequence_s = header
        for movement in sequence:
            # get sequence data
            axis = movement[0]
            direction = movement[1]
            duration = movement[2]
            if duration > 9999:
                duration = 9999
            duration_s = str(duration).zfill(4)
            # compile string
            sequence_s += axis + direction + duration_s + ';'
            # add a pause after each movement
            if direction != 'P':
                sequence_s += axis + 'P' + '0200' + ';'
        sequence_s += footer
        return sequence_s

    def send_sequence(self, sequence):
        sequence_str = self.sequence_to_sequence_str(sequence)
        self.send_sequence_str(sequence_str)

    def mv_home(self):
        sequence = [('X', 'F', 8000),
                    ('Y', 'F', 9000)]
        self.send_sequence(sequence)

    def mv_grab_position(self):
        sequence = [('X', 'B', 2000),
                    ('Y', 'B', 2500)]
        self.send_sequence(sequence)

    def mv_grab(self):
        sequence = [('Z', 'F', 4000),
                    ('Z', 'P', 2000),
                    ('Z', 'B', 4070)]
        self.send_sequence(sequence)

    def mv_drop_position(self):
        sequence = [('X', 'B', 6000),
                    ('Y', 'B', 6000)]
        self.send_sequence(sequence)

    def mv_release(self):
        sequence = [('Z', 'F', 1000),
                    ('Z', 'P', 2000),
                    ('Z', 'B', 1000)]
        self.send_sequence(sequence)

    def mv_z_step_up(self):
        sequence = [('Z', 'B', 100)]
        self.send_sequence(sequence)

    def mv_z_step_down(self):
        sequence = [('Z', 'F', 100)]
        self.send_sequence(sequence)


def main():
    # command line arguments
    parser = argparse.ArgumentParser(
                    prog = "Claw Motor Controler",
                    description = "Controls varios movements of a claw machine")
    parser.add_argument('--device_path', default='/dev/ttyUSB0')
    parser.add_argument('--action', choices=['home', 'give_food', 'drop', 'move_grab', 'grab', 'move_drop_position', 'z_up', 'z_down'])
    args = parser.parse_args()
    # initialize mover
    claw_mover = ClawMover(args.device_path)
    claw_mover.connect()
    # do movements
    if hasattr(args, 'action') and args.action:
        if args.action == 'home':
            claw_mover.mv_home()
        elif args.action == 'give_food':
            claw_mover.mv_grab_position()
            time.sleep(1)
            claw_mover.mv_grab()
            time.sleep(1)
            claw_mover.mv_drop_position()
            time.sleep(0.5)
            claw_mover.mv_release()
            time.sleep(2.0)
            claw_mover.mv_home()
        elif args.action == 'drop':
            claw_mover.mv_release()
        elif args.action == 'move_grab':
            claw_mover.mv_grab_position()
        elif args.action == 'move_drop_position':
            claw_mover.mv_drop_position()
        elif args.action == 'grab':
            claw_mover.mv_grab()
        elif args.action == 'z_down':
            claw_mover.mv_z_step_down()
        elif args.action == 'z_up':
            claw_mover.mv_z_step_up()
            

if __name__ == "__main__":
    main()
