all: run

run: cam
	./cam -c 5

faceDetect: cam.c
	gcc -O2 -Wall `pkg-config --cflags --libs libv4l2` cam.c -o cam
