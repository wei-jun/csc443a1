all : create_random_file get_histogram

create_random_file : create_random_file.o
	gcc -Wall -g -o create_random_file create_random_file.o

get_histogram : get_histogram.o
	gcc -Wall -g -o get_histogram get_histogram.o

%.o : %.c
	gcc -Wall -g -c $<
	
clean : 
	rm create_random_file get_histogram *.o