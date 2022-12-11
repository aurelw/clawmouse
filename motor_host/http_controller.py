#!/usr/bin/python3

import time
import threading

from http.server import SimpleHTTPRequestHandler, HTTPServer
from urllib.parse import parse_qs

from motor_controller import ClawMover


class ClickPayer:

    def __init__(self, device_path="/dev/ttyUSB0"):
        self._click_thresh = 5
        self._n_clicks = 0
        self._claw = ClawMover(device_path, do_disconnect=True)
        self._thread = None
        self._currently_paying = False

    def do_click(self):
        if self._n_clicks < self._click_thresh:
            self._n_clicks += 1
        if self._n_clicks >= self._click_thresh:
            self._n_clicks = 0
            self.pay()

    def pay(self):
        def run_pay():
            self._claw.mv_grab_position()
            time.sleep(1)
            self._claw.mv_grab()
            time.sleep(1)
            self._claw.mv_drop_position()
            time.sleep(0.5)
            self._claw.mv_release()
            time.sleep(2.0)
            self._claw.mv_home()
            # don't pay right away. wait for new clicks
            self._currently_paying = False
            self._n_clicks = 0
        # only start thread if not yet running
        if self._thread == None or not self._thread.is_alive():
            self._thread = threading.Thread(target=run_pay)
            self._currently_paying = True
            self._thread.start()


click_payer = ClickPayer(device_path="/dev/ttyUSB0")


class MyHandler(SimpleHTTPRequestHandler):

    def do_GET(self):
        qs = {}
        path = self.path
        if '?' in path:
            path, tmp = path.split('?', 1)
            qs = urlparse.parse_qs(tmp)
        if "image_click" in path:
            click_payer.do_click()


if __name__ == "__main__":
    try:
        server = HTTPServer(('localhost', 7777), MyHandler)
        print('Started http server')
        server.serve_forever()
    except KeyboardInterrupt:
        print('^C received, shutting down server')
        server.socket.close()

