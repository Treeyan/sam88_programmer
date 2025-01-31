#    Makefile 

target=sam88_programmer

all: $(target)	

$(target): main.c parport_driver.c sam88chip.c *.h
	gcc -g -O0 -o $(target) main.c parport_driver.c sam88chip.c
