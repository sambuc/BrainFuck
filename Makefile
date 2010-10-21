#
# Created by Lionel Sambuc on 21.oct.2010.
# Copyright 2010-2011. All rights reserved.
#

all: prepare $(patsubst %.c,build/%.o,$(wildcard *.c))
	gcc -o build/bfi build/*.o

.PHONY: prepare
prepare:
	-mkdir build

build/%.o: %.c *.h
	gcc -c -o $@ $<

clean:
	-rm -rf build