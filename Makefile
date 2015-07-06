FIRMWARE=sparkcopter.bin

.PHONY: clean

all: $(FIRMWARE)

$(FIRMWARE):
	particle compile . --saveTo $(FIRMWARE) --deviceType $(DEVICE_TYPE)

install: $(FIRMWARE)
	particle flash $(DEVICE_NAME) $(FIRMWARE)

install-usb: $(FIRMWARE)
	particle flash --usb $(FIRMWARE)

clean:
	rm -f *.bin
