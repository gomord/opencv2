all:
	gcc -c -fPIC motor.c -o motor.o -lrt -lwiringPi
	gcc -shared -Wl,-soname,libmotor.so.1 -o libmotor.so.1.0.1  motor.o
