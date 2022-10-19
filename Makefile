.PHONY: compile upload

compile:
	arduino-cli compile --no-color

upload: compile
	arduino-cli upload --no-color

monitor:
	arduino-cli monitor -p /dev/ttyACM0

install-libs:
	arduino-cli lib install "Adafruit Motor Shield library"

c: compile
u: upload
m: monitor
