CC=gcc
CFLAGS=

framesize ?= 0
varmemsize ?= 0

myshell: shell.c interpreter.c shellmemory.c
	$(CC) $(CFLAGS) -c -g shell.c interpreter.c shellmemory.c kernel.c pcb.c ready_queue.c -D FRAMESIZE=${framesize} -D VARMEMSIZE=${varmemsize}
	$(CC) $(CFLAGS) -o myshell shell.o interpreter.o shellmemory.o kernel.o pcb.o ready_queue.o -D FRAMESIZE=${framesize} -D VARMEMSIZE=${varmemsize}

clean: 
	rm myshell; rm *.o
