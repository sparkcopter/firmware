FIRMWARE=sparkcopter.bin
DEVICE_USB=/dev/tty.usbmodem1411

DEVICE_TYPE=photon
DEVICE_NAME=proton

# DEVICE_TYPE=core
# DEVICE_NAME=shark

.PHONY: clean

all: $(FIRMWARE)

$(FIRMWARE):
	particle compile . --saveTo $(FIRMWARE) --deviceType $(DEVICE_TYPE)

install: $(FIRMWARE)
	particle flash $(DEVICE_NAME) $(FIRMWARE)

install-usb: $(FIRMWARE)
	stty -f $(DEVICE_USB) 14400
	particle flash --usb $(FIRMWARE)

clean:
	rm -f *.bin
