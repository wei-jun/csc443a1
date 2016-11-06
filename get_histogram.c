#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>

/**
* file_ptr : the file pointer, ready to be read from.
* hist: an array to hold 26 long integers.  hist[0] is the
*       number of 'A', and hist[1] is the number of 'B', etc.
* block_size: the buffer size to be used.
* milliseconds: time it took to complete the file scan
* total_bytes_read: the amount data in bytes read
*
* returns: -1 if there is an error.
*/
     
int get_histogram(
    FILE *file_ptr, 
    long hist[], 
    long block_size, 
    long *milliseconds, 
    long *total_bytes_read) {

	int i, j;
	struct timeb t_start, t_current;
	long start_in_ms, current_in_ms;
	long t_diff = 0;
	long total_bytes = 0;
	char buf[block_size];
	bzero(buf, block_size);

	// initial hist[]
	for (i=0; i<26; i++) {
		hist[i] = 0;
	}

	ftime(&t_start);
	start_in_ms = t_start.time * 1000 + t_start.millitm;
	long c = fread(buf, 1, block_size, file_ptr);
	if (c < 0) {
		return -1;
	}
	ftime(&t_current);
    current_in_ms = t_current.time * 1000 + t_current.millitm;

	while (!feof(file_ptr)) {
		total_bytes += c;
		t_diff += current_in_ms - start_in_ms;
		for (i=0; i<c; i++) {
			j = (int)(buf[i] - 'A');
			if (j > 25) {
				return -1;
			} else {
				hist[j]++;
			}			
		}
		ftime(&t_start);
	    start_in_ms = t_start.time * 1000 + t_start.millitm;
	    c = fread(buf, 1, block_size, file_ptr);
	    ftime(&t_current);
        current_in_ms = t_current.time * 1000 + t_current.millitm;
	}
	*milliseconds = t_diff;
	*total_bytes_read = total_bytes;

	return 0;
}


/**
*implement the command get_histogram with the following syntax
*$ get_histogram <filename> <block size>
*/
int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: get_histogram <filename> <block size>\n");
		exit(1);
	}

	FILE *file_ptr;
	if ((file_ptr = fopen(argv[1], "r")) == NULL) {
		printf("cannot read file %s\n", argv[1]);
		exit(-1);
	}

	long block_size = atoi(argv[2]);
	if (block_size < 0) {
		printf("block_size should be positive.\n");
		exit(-1);
	}

	int i;
	long hist[26];
	long milliseconds;
	long filelen;

    //Compute the histogram using block_size buffers
    int ret = get_histogram( file_ptr, 
                             hist, 
                             block_size,
                             &milliseconds,
                             &filelen);
    if (ret < 0) {
    	exit(-1);
    } 
    printf("Computed the histogram in %ld ms.\n", milliseconds);
    for(i=0; i < 26; i++) {
    printf("%c : %ld\n", 'A' + i, hist[i]);
    }
    printf("filelen=%ld\n", filelen);
    printf("time_to_read=%ld\n", milliseconds);
    printf("Data rate: %f Bps\n", (double)filelen/milliseconds * 1000);

    return 0;
}

