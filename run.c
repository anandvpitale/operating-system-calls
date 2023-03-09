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
	char *mode = argv[2];
	char *endptr1;
	char *endptr2;
	int block_size = strtol(argv[3], &endptr1, 10);
	int block_count = strtol(argv[4], &endptr2, 10);
	
	int fd;
	uint64_t file_size;
	unsigned int xor_buf;
	double start, end;
	unsigned int xor_total=0;
	
	file_size = block_size * block_count;
	
	void* memory = malloc(block_size);
	
	start = now();
	
	if(strcmp(mode, "-r") == 0){
		fd = open(file_name, O_RDONLY); 
		for(int i = 0; i < block_count; i++){
			
			read(fd, memory, block_size);
			xor_buf = xorbuf((unsigned int*) memory,block_size);
			xor_total ^= xor_buf;
		}
	}else if(strcmp(mode, "-w") == 0){
		fd = open(file_name, O_WRONLY); 
		for(int i = 0; i < block_count; i++){
			
			write(fd, memory, block_size);
		}
	}else if(strcmp(mode, "-l") == 0){
		fd = open(file_name, O_WRONLY); 
		for(int i = 0; i < block_count; i++){
			
			lseek(fd, 0, i%2==0?SEEK_SET:SEEK_END);
	}
	}else{
		printf("Unknown mode type\n");
		exit(1);
	}
	
	if(fd < 0){
		printf("File does not exist\n");
		exit(1);
	}
	
	
	printf("file_size: %llu\n",  file_size);
	
	free(memory);
	close(fd);
	end = now();
	
	double time = end - start;
	printf("Total time: %f\n", time);
	
	printf("xor = %08x\n", xor_total);
	
	double mib = (file_size / time) / (1024 * 1024);
	printf("Performance: %f Mib/s\n", mib);
	double b_sec = (file_size / time);
	printf("Performance: %f bytes/s\n", b_sec);
	
	printf("\n");
	
	return 0;
}






