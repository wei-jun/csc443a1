#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>


/**
* populate a random array (which is already
* allocated with enough memory to hold n bytes.
*/
void random_array(char *array, long bytes) {
	int i;
	for (i=0; i<bytes; i++) {
		array[i] = 'A' + (rand() % 26);
	}
	return;
}

/**
*implement the command create_random_file with the following syntax
*$ create_random_file <filename> <total bytes> <block size>
*/
int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("usage: create_random_file <filename> <total make bytes <block size>\n");
		exit(-1);
	}

	long total_bytes = atoi(argv[2]);
	long block_size = atoi(argv[3]);
	if (total_bytes <= 0 || block_size <= 0) {
		printf("total_bytes and block_size should be positive.\n");
		exit(-1);
	}
	
	char buf[block_size];

	FILE *fp = fopen(argv[1], "w");

	int i;
	struct timeb t_start, t_current;
	long start_in_ms, current_in_ms;
	long t_diff = 0;

	for (i=0; i<(total_bytes/block_size); i++) {
		random_array(buf, block_size);
		printf("array: %s\n", buf);

		ftime(&t_start);
		start_in_ms = t_start.time * 1000 + t_start.millitm;
	    fwrite(buf, 1, block_size, fp);
	    fflush(fp);
	    ftime(&t_current);
		current_in_ms = t_current.time * 1000 + t_current.millitm;
		t_diff += current_in_ms - start_in_ms;

	}

	bzero(buf, block_size);
	random_array(buf, total_bytes % block_size);
	printf("array: %s\n", buf);
	ftime(&t_start);
	start_in_ms = t_start.time * 1000 + t_start.millitm;
	fwrite(buf, 1, total_bytes % block_size, fp);
	fflush(fp);
	ftime(&t_current);
	current_in_ms = t_current.time * 1000 + t_current.millitm;
	t_diff += current_in_ms - start_in_ms;

	printf("total_bytes = %ld\n", total_bytes);
	printf("block_size = %ld\n", block_size);
	printf("time_to_write = %ld\n", t_diff);
	if (t_diff == 0) {
		printf("write_data_rate = %s\n", "inf");
	}
	else{
		printf("write_data_rate = %ld\n", 1000*total_bytes/t_diff);
	}

	fclose(fp);
    return 0;
}