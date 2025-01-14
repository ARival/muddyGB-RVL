CC = lcc

MODULES = sound music muddygb_rvl
OBJECTS = $(addsuffix .o, $(MODULES))

all: muddygb_rvl.gb
	@echo MuddyGB_RVL.gb has been compiled

muddygb_rvl.gb: $(OBJECTS)
	$(CC) -Wl-yp0x143=0x80 $(OBJECTS) -o $@

%.o: %.c %.h
	$(CC) -c $< -o $@

clean:
	$(RM) *.gb *.o *~
