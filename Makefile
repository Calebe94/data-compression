PORT=/dev/ttyUSB0
BAUD=115200

compile:
	pio run

flash:
	pio run -t upload

partition:
	pio run -t uploadfs

monitor:
	pio device monitor --port ${PORT} --baud ${BAUD} --raw -f colorize
