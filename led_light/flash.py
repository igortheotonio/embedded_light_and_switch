import sys
import os
from time import sleep

cmds = [
    "nrfjprog --eraseall -f nrf52 && nrfjprog --program build/zephyr/zephyr.hex -f nrf52 && nrfjprog --reset -f nrf52"]

while(1) :
    os.system(cmds[0])
    sleep(2)
