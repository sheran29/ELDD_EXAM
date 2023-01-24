obj-m:= prog.o

Kern_Dir = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

default:
	make -C $(Kern_Dir) M=$(PWD) modules
	gcc -o normal normal.c

clean:
	make -C $(Kern_Dir) M=$(PWD) clean
	rm -rf normal                                                 
                                                                                                                     
