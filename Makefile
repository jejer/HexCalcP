.SECONDARY: $(OBJ) $(ELF) $(HP)

INCLUDE_PATH= "$(HPGCC)\include"
LIBS_PATH= "$(HPGCC)\lib"

export CC= arm-elf-gcc
export AR= arm-elf-ar
export ELF2HP= elf2hp

export C_FLAGS= -std=c99 -mtune=arm920t -mcpu=arm920t \
	-mlittle-endian -fomit-frame-pointer -Wall \
	-Os -I$(INCLUDE_PATH) -L$(LIBS_PATH)
# export C_FLAGS= -std=c99 -Wall -Os -I$(INCLUDE_PATH) -L$(LIBS_PATH) \
# 	-mtune=arm920t -mcpu=arm920t -mlittle-endian -msoft-float \
# 	-fomit-frame-pointer -fdata-sections -ffunction-sections
# add this for Thumb interwork mode:
# export C_FLAGS += -mthumb-interwork -mthumb

export LD= arm-elf-ld
export LD_FLAGS= -L$(LIBS_PATH) -T MMUld.script $(LIBS_PATH)/crt0.o 
export LIBS= -lwin -lggl -lhpg -lhplib -lgcc

SRC = $(wildcard *.c)

OBJ = $(SRC:%.c=%.o)

ELF = $(SRC:%.c=%.elf)

HP = $(SRC:%.c=%.hp)

all: hp2aplet2 HexCalcP.apt

hp2aplet2:
ifeq (, $(shell which hp2aplet2))
	cp build_tools/hp2aplet2.exe $(HPGCC)\bin\hp2aplet2.exe
endif

clean:
	rm -rf *.o
	rm -rf *.elf
	rm -rf *.hp
	rm -rf *.apt


%.o: %.c
	$(CC) $(C_FLAGS) -c $< -o $@

%.elf: $(OBJ)
	$(LD) $(OBJ) $(LD_FLAGS) $(LIBS) -o $@

%.hp: %.elf
	$(ELF2HP) $< $@

%.apt: %.hp
	hp2aplet2.exe $< $@ "HexCalcP"



