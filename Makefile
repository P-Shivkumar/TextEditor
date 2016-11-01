project: main.c command.c command.h display.c display.h
	cc -Wall main.c -o project `pkg-config --cflags --libs gtk+-2.0`
