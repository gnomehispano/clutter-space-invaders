SOURCES =		\
	common.h	\
	invader.c	\
	invader.h	\
	laser.c		\
	main.c		\
	starship.c	\
	starship.h

all: space-invaders

space-invaders: $(SOURCES)
	gcc -o space-invaders `pkg-config --cflags --libs clutter-gtk-1.0 clutter-1.0` $(SOURCES)

clean:
	- rm space-invaders
	- rm *~
