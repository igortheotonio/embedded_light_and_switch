import sys
import os

cmds = [
    "ninja",
    "nrfjprog --eraseall -f nrf52", 
    "nrfjprog --program zephyr/zephyr.hex -f nrf52", 
    "nrfjprog --reset -f nrf52"]

for cmd in cmds:
    os.system(cmd)