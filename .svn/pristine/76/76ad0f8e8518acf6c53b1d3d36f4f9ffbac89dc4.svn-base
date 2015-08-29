# EPOS Image Makefile

include	../makedefs

SYSTEM := $(shell find . -name $(MACH)_\* -and -not -name $(MACH)_boot)
IMAGE  := $(addsuffix .img,$(APPLICATION))
FLASH  := $(addsuffix .hex,$(APPLICATION))
OUTPUT := $(addsuffix .out,$(APPLICATION))

all:		strip $(IMAGE)

run:		strip $(IMAGE)
ifeq ($(NODES),1)
		$(EMULATOR) $(IMAGE) | $(TEE) $(OUTPUT)
else
#		$(TERM) "$(EMULATOR) $(IMAGE) -net nic,model=pcnet,macaddr=56:34:12:0:54:08 -net dump,file=r-$(APPLICATION).pcap -net socket,listen=:1234 -net user,net=10.0.1.0/24,host=10.0.1.1,dhcpstart=10.0.1.8 | $(TEE) r-$(OUTPUT)
		$(TERM) "$(EMULATOR) $(IMAGE) -net nic,model=pcnet,macaddr=56:34:12:0:54:08 -net dump,file=r-$(APPLICATION).pcap -net socket,listen=:1234 | $(TEE) r-$(OUTPUT) \
		&& $(TCPDUMP) r-$(APPLICATION).pcap > r-$(APPLICATION).net \
		&& read -p 'Press [Enter] key to close ...'" &
		sleep 2
		$(TERM) "$(EMULATOR) $(IMAGE) -net nic,model=pcnet,macaddr=56:34:12:0:54:09 -net dump,file=s-$(APPLICATION).pcap -net socket,connect=:1234 | $(TEE) s-$(OUTPUT) \
		&& $(TCPDUMP) s-$(APPLICATION).pcap > s-$(APPLICATION).net \
		&& read -p 'Press [Enter] key to close ...'" &
endif

debug:		$(IMAGE)
		$(EMULATOR) $(IMAGE) | $(TEE) $(OUTPUT) &
		$(DEBUGGER) $(APP)/$(APPLICATION)

flash:		$(FLASH)

strip:		$(SYSTEM) $(APPLICATION)
		@for file in $^; do $(STRIP) "$${file}"; done;
		
clean:
		$(CLEAN) $(SYSTEM) $(APPLICATION)
