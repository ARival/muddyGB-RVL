CC = lcc

MODULES = input sound music boueux
OBJECTS = $(addsuffix .o, $(MODULES))

all: boueux.gb
	@echo Boueux.gb has been compiled

boueux.gb: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.c %.h
	$(CC) -c $< -o $@

clean:
	$(RM) *.gb *.o *~
