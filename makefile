SOURCES := gol.c

run:
	make app && ./app

app: $(SOURCES)
	gcc $(SOURCES) -o app -std=c17 -Wall -lSDL2 -lSDL2_image
