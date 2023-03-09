#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/time.h>
#include <stdint.h>
#include <sys/stat.h>


double now() {
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

unsigned int xorbuf(unsigned int *buffer, int size) {
    unsigned int result = 0;
    for (int i = 0; i < size; i++) {
        result ^= buffer[i];
    }
    return result;
}

int main(int argc, char *argv[]){
	//Input file_name, mode, block_size, block_count
	char *file_name = argv[1];
	int block_size = 32000;
	int block_count;
	
	int fd;
	uint64_t file_size;
	unsigned int xor_buf;
	double start, end;
	unsigned int xor_total=0;
	
	struct stat st;
	stat(file_name, &st);
	file_size = st.st_size;
	
	void* memory = malloc(block_size);
	
	start = now();
	
	fd = open(file_name, O_RDONLY); 
	
	block_count = file_size / block_size;
	
	for(int i = 0; i < block_count; i++){
		
		read(fd, memory, block_size);
		xor_buf = xorbuf((unsigned int*) memory,block_size);
		xor_total ^= xor_buf;
	}
	
	if(fd < 0){
		printf("File does not exist\n");
		exit(1);
	}
	
	free(memory);
	close(fd);
	end = now();
	
	double time = end - start;
	
	printf("xor = %08x\n", xor_total);
	printf("Total time: %f\n", time);
	
	double mib = (file_size / time) / (1024 * 1024);
	printf("Performance: %f Mib/s\n", mib);
	
	return 0;
}