FIRMWARE=sparkcopter.bin

.PHONY: clean

all: $(FIRMWARE)

$(FIRMWARE):
	spark compile src --saveTo $(FIRMWARE)

install: $(FIRMWARE)
	spark flash $(SPARK_NAME) $(FIRMWARE)

install-usb: $(FIRMWARE)
	spark flash --usb $(FIRMWARE)

clean:
	rm -f *.bin
