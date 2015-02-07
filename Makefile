FIRMWARE=sparkcopter.bin
SPARK_NAME=shark

.PHONY: clean

all: $(FIRMWARE)

$(FIRMWARE):
	spark compile . --saveTo $(FIRMWARE)

install: $(FIRMWARE)
	spark flash $(SPARK_NAME) $(FIRMWARE)

clean:
	rm *.bin
