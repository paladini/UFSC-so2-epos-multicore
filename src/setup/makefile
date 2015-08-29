# EPOS Setup Makefile

include ../../makedefs

TARGET := $(if $(shell find $(MACH)* 2> /dev/null), install)

all:		$(TARGET)

$(MACH)_setup:	$(MACH)_setup.o
		$(LD) $(LDFLAGS) -L$(CCLIB) --omagic --section-start .init=$(SETUP_ADDR) -o $@ $^ -l$(LINIT) -l$(LMACH) -l$(LARCH) -l$(LUTIL) -lgcc

install:	$(MACH)_setup
		$(INSTALL) $< $(IMG)
		
clean:
		$(CLEAN) *.o *_setup
