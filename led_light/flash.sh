#!/bin/bash
nrfjprog --eraseall -f nrf52
nrfjprog --program build/zephyr/zephyr.hex -f nrf52
nrfjprog --reset -f nrf52
