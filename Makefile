all: MonPG4 MonPG5 MonPG6

MonPG4: MonPG4.c
	gcc MonPG4.c -o MonPG4

MonPG5: MonPG5.c
	gcc MonPG5.c -o MonPG5

MonPG6: MonPG6.c
	gcc -o MonPG6 MonPG6.c  stb_image/stb_image.h stb_image/stb_image_resize.h stb_image/stb_image_write.h -lm

clean:
	rm MonPG4 MonPG5 MonPG6
