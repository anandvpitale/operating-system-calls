#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <filename> <block_size>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int block_size = atoi(argv[2]);
    double elapsed_time_final;

    // start with a small file size and double it until it takes between 5 and 15 seconds to read
    int file_size = block_size;
    int i = 0;
    while (1) {
        i = i + 1;
        clock_t start_time = clock();
        FILE* f = fopen(filename, "rb");
        if (f == NULL) {
            perror("Error opening file");
            return 1;
        }
        for (int i = 0; i < file_size / block_size; i++) {
            char* block = malloc(block_size);
            if (block == NULL) {
                perror("Error allocating memory");
                fclose(f);
                return 1;
            }
            size_t n = fread(block, 1, block_size, f);
            if (n < block_size && !feof(f)) {
                perror("Error reading file");
                fclose(f);
                free(block);
                return 1;
            }
            free(block);
        }
        fclose(f);
        clock_t end_time = clock();
    
        double elapsed_time = (double)((end_time - start_time) * 100000) / CLOCKS_PER_SEC;
        printf("Elapsed Time interation %d: %f \n", i, elapsed_time);
        if (elapsed_time > 5.0 && elapsed_time < 15.0) {
            elapsed_time_final = elapsed_time;
            break;
        }
        file_size *= 2;
    }

    // calculate the number of blocks needed to read the file
    int block_count = file_size / block_size;

    // calculate the read performance in MiB/s or bytes/sec
    double read_performance = (double)file_size / elapsed_time_final;

    printf("Block count: %d\n", block_count);
    printf("File size: %d bytes\n", file_size);
    printf("Read performance: %.2f bytes/sec\n", read_performance);

    return 0;
}
