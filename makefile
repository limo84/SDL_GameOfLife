SOURCES := $(shell find . -name "*.c")

all: $(SOURCES)
	gcc $(SOURCES) -o app -lSDL2 -lSDL2_image