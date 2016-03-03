CC = gcc
CFLAGS = -g
RM = rm

EXEM = master
EXES = slave
SRCSM = master.c monitor.c condition.c semaphore.c
SRCSS = slave.c monitor.c condition.c semaphore.c
OBJSM = ${SRCSM:.c=.o}
OBJSS = ${SRCSS:.c=.o}

.c:.o
	$(CC) $(CFLAGS) -c $<

all : $(EXEM) $(EXES)

$(EXEM) : $(OBJSM)
	$(CC) -o $@ $(OBJSM)

$(OBJSM) : master.h monitor.h

$(EXES) : $(OBJSS)
	$(CC) -o $@ $(OBJSS)

$(OBJSS) : master.h monitor.h

clean :
	$(RM) -f $(EXES) $(EXEM) $(OBJSS) $(OBJSM)

cleanexe :
	$(RM) -f slave master




