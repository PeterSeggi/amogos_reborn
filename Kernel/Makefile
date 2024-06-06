include Makefile.inc

KERNEL=kernel.bin
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

ifeq ($(MEMORY_MANAGER), buddy)
    MEMORY_MANAGER_DIR = buddyMemoryManager
else
    MEMORY_MANAGER_DIR = memoryManager

endif

MEMORY_MANAGER_SOURCES = $(wildcard $(MEMORY_MANAGER_DIR)/*.c)

SOURCES_C = $(wildcard *.c)
OBJECTS_C = $(SOURCES_C:.c=.o)
OBJECTS = $(OBJECTS_C) $(OBJECTS_ASM) $(MEMORY_MANAGER_SOURCES:.c=.o)

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin

.PHONY: all clean
