# EPOS Machine Common Packages Makefile

include ../../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc))

all:		$(LIBMACH)

$(LIBMACH):	$(LIBMACH)($(OBJS))	

clean:
		$(CLEAN) *.o
