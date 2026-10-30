ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment.")
endif

include $(DEVKITPPC)/wii_rules

TARGET   := boot
GRAPHICS := gfx
PNGFILES := $(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))
export GFXDEPS := $(PNGFILES:.png=.h)

LIBS     := -lgrrlib -lpngu -lpng -lz -lwiiuse -lbte -logc -lm
CFLAGS   := -g -O2 -Wall $(MACHDEP) -I. -I$(GRAPHICS)

RAW2C    := /opt/devkitpro/tools/bin/raw2c

all: $(GFXDEPS) $(TARGET).dol

%.h: $(GRAPHICS)/%.png
	@echo "Konvertujem $< do C pola..."
	@$(RAW2C) $<

$(TARGET).dol: $(TARGET).elf

$(TARGET).elf: main.c $(GFXDEPS)
	$(CC) $(CFLAGS) main.c $(LDFLAGS) $(LIBS) -o $@

clean:
	rm -f *.elf *.dol gfx/*.h
