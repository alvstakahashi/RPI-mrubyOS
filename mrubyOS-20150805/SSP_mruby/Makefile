CROSS= arm-none-eabi-

CC= $(CROSS)gcc
LD= $(CROSS)gcc
OBJCOPY= $(CROSS)objcopy
OBJDUMP= $(CROSS)objdump
SIZE= $(CROSS)size
READELF= $(CROSS)readelf

STARTUP= startup.o
OBJS= main.o
OBJS += rbcall.o
OBJS += rb_main.o
OBJS += setup.o
OBJS += mrb_read.o

# systemcall
OBJS += syscalls.o

# RaspberryPi library
OBJS+= rpi_lib/rpi_init.o rpi_lib/gpio/gpio.o rpi_lib/timer/timer.o rpi_lib/delay/delay.o
OBJS+= rpi_lib/serial/serial.o
OBJS+= rpi_lib/bss/clearbss.o

#ssp liblary
OBJS+= ssp/kernel_cfg.o ssp/startup.o ssp/task.o ssp/task_manage.o
OBJS+= ssp/alarm.o ssp/cyclic.o ssp/time_event.o ssp/time_manage.o
OBJS+= arch/RPI/target_asm.o

#ssp include
INCSSP = -I./ssp -I./include -I./arch/RPI
MRUBY_ROOT = ../mruby

INCDIR = -I $(MRUBY_ROOT)/include
ULIBDIR = -L $(MRUBY_ROOT)/build/ARM/lib
ULIBS = -lmruby
#LIB=-L C:\yagarto\lib\gcc\arm-none-eabi\4.7.2 -L C:\yagarto\arm-none-eabi\lib

CFLAGS = -mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfloat-abi=softfp -O0 -ggdb -g

CFLAGS+=  -mlittle-endian $(INCSSP) $(INCDIR)
CFLAGS+= -nostartfiles
CFLAGS+= -ffreestanding
#CFLAGS+= -O2 
CFLAGS += -O0 -g -gdwarf-2
# CFLAGS+= -marm -mcpu=arm1176jzf-s 
CFLAGS += -D$(RPITYPE)


########################
all:	rpi-micon.img

rpi-micon.elf:	$(STARTUP) $(OBJS)
#	$(LD) -static -nostartfiles   $^  -T memmap -o $@  $(ULIBDIR) $(ULIBS) $(LIB) -lc -lm -lgcc 
	$(LD) -static -nostartfiles -T rpiMicon.lds $^ -o $@  $(ULIBDIR) $(ULIBS) $(LIB) -lc -lm -lgcc 
	$(OBJDUMP) -D rpi-micon.elf > rpi-micon.disas
	$(SIZE) rpi-micon.elf > rpi-micon.size
	$(READELF) -a rpi-micon.elf > rpi-micon.readelf

.SUFFIXES: .elf .img

.elf.img:
	$(OBJCOPY) -O binary $< $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.S.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean::
	$(RM) -f *.o *.img *.elf */*.o */*/*.o 
	$(RM) -f tags *~

tags::
	ctags *.[chS]
