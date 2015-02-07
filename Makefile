.PHONY: clean

all: firmware.bin

firmware.bin:
	spark compile . --saveTo firmware.bin

install: firmware.bin
	spark flash shark firmware.bin

clean:
	rm *.bin
