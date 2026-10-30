ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment.")
endif

include $(DEVKITPPC)/wii_rules

TARGET := boot
GRAPHICS := gfx
PNGFILES := $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
export GFXDEPS := $(PNGFILES:.png=.h)

LIBS := -lgrrlib -lpngu -lpng -lz -lwiiuse -lbte -logc -lm
CFLAGS := -g -O2 -Wall $(MACHDEP) -I. -I$(GRAPHICS)

all: $(GFXDEPS) $(TARGET).down

%.h: $(GRAPHICS)/%.png 
@echo "Converting $< to a C array..." 
@raw2c $<

$(TARGET).down: $(TARGET).elf

$(TARGET).elf: main.c $(GFXDEPS) 
$(CC) $(CFLAGS) main.c $(LDFLAGS) $(LIBS) -o $@

clean: 
rm -f *.elf *.dol gfx/*.h
