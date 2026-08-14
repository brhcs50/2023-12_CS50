// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor =
        atof(argv[3]); //-- "atof" converts string to float, from stdlib.h, the factor is the third argument at command line

    // TODO: Copy header from input file to output file
    //-- fread: size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    //-- ptr, which is the address (of the first byte) of memory into which to read the data,
    //-- size, which is the size (in bytes) of the type of data to read,
    //-- nmemb, which is the number of those types to read at once, and
    //-- stream, which is the pointer to a FILE returned by fopen.
    uint8_t header[HEADER_SIZE]; //-- create array of size HEADER_SIZE defined above, "uint8_t" holds an unsigned numerical value

    fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);

    // TODO: Read samples from input file and write updated data to output file
    // fwrite: size_t fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
    // exmample:
    // char c;
    // while (fread(&c, sizeof(char), 1, input))
    // {
    //     fwrite(&c, sizeof(char), 1, output);
    // }
    int16_t buffer; //-- declaring "buffer" as 2 bytes, 16 bits
    // You can then use &buffer as an argument to fread or fwrite to read into or write from the buffer.
    // (Recall that the & operator is used to get the address of the variable.)

    while (fread(&buffer, sizeof(int16_t), 1, input) == 1) //-- while there is still "1" 16 bit part to read (==1), then do this
    {
        buffer *= factor;
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
