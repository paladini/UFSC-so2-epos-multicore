# EPOS Source Makefile

include	../makedefs

SUBDIRS := utility architecture machine abstraction setup boot system init

all:		$(SUBDIRS)

$(SUBDIRS):	FORCE
		(cd $@ && $(MAKE))

clean:
		make MAKE:="$(MAKECLEAN)" $(SUBDIRS)

FORCE:
