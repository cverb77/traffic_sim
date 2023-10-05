EXECS = simulation #name of file with main goes here 
OBJS = simulation.o Animator.o VehicleBase.o Ingest.o #list files to compile


#### use next two lines for mathcs* machines:
CC = g++
CCFLAGS = -std=c++17

all: $(EXECS)

simulation: $(OBJS) #compiles the object files with a file containing a main, linking them and spitting out an executable
	$(CC) $(CCFLAGS) -g $^ -o $@

%.o: %.cpp *.h #creates object files with headers being changed
	$(CC) $(CCFLAGS) -c $<

%.o: %.cpp #creates object files without headers being changed
	$(CC) $(CCFLAGS) -c $<

clean: #deletes all of object files and the executables
	/bin/rm -f a.out $(OBJS) $(EXECS)
