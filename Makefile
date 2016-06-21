CC = lcc

MODULES = sound music muddygb_dsd
OBJECTS = $(addsuffix .o, $(MODULES))

all: muddygb_dsd.gb
	@echo MuddyGB_DSD.gb has been compiled

muddygb_dsd.gb: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c %.h
	$(CC) -c $< -o $@

clean:
	$(RM) *.gb *.o *~
