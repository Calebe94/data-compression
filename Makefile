DATA_DIR=data/
PARTITION_BIN=spiffs.bin
PORT=/dev/ttyUSB0
BAUD=115200

all: compile flash monitor

compile:
	pio run

flash:
	pio run -t upload

partition:
	pio run -t uploadfs

monitor:
	pio device monitor --port ${PORT} --baud ${BAUD} --raw -f colorize

get_fs:
	. ${IDF_PATH}/export.sh; parttool.py --port "${PORT}" --baud 921600 read_partition --partition-type=data --partition-subtype=spiffs --output "${PARTITION_BIN}"

unpack: ${DATA_DIR}
	mkspiffs -u ${DATA_DIR} -b 4096 -p 256 ${PARTITION_BIN}

extract: get_fs unpack

.PHONY: compile flash partition monitor get_fs unpack extract
