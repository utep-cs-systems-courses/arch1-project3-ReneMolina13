# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:msquares.elf

#additional rules for files
msquares.elf: ${COMMON_OBJECTS} main.o msquares.o wdt_handler.o switch_handler.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: msquares.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf

main.o: msquares.h
msquares.o: msquares.h
