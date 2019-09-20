

#compile

CC = g++
LD = g++
AR = ar


#linkedit
DEBUG = -D _MY_DEBUG_
CFLAGS = -Wall -O3
LFLAGS = -Wall

AFLAGS = -r

GRAFOS = grafos
GRAFOSOBJS = grafos.o

GRAFOSMAIN = grafosMain
GRAFOSMAINOBJS = grafos.o main.o

EXECS = $(GRAFOS) $(GRAFOSMAIN)

ALL = $(EXECS)

#Regra Implicita
.c.o: $(CC) $(CFLAGS) -c $<

all: $(EXECS)

grafos: $(GRAFOSOBJS)
	# $(LD) $(LFLAGS) -o $@ $(GRAFOSOBJS)

grafosMain: $(GRAFOSMAINOBJS)
	$(LD) $(LFLAGS) -o $@ $(GRAFOSMAINOBJS)

clean:
	rm -f *.o $(ALL)
