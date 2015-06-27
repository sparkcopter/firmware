FIRMWARE=sparkcopter.bin

.PHONY: clean

all: $(FIRMWARE)

$(FIRMWARE):
	particle compile . --saveTo $(FIRMWARE)

install: $(FIRMWARE)
	particle flash $(SPARK_NAME) $(FIRMWARE)

install-usb: $(FIRMWARE)
	particle flash --usb $(FIRMWARE)

clean:
	rm -f *.bin
