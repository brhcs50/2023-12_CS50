#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    // Check for invalid usage
    if (argc < 3)
    {
        printf("Usage: reverse [input.wav] [output.wav]\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    //-- file open read only argv[1];
    //-- verify file is opened successfully;
    char *inputfile = argv[1];
    FILE *inpointer = fopen(inputfile, "rb");
    if (inpointer == NULL)
    {
        printf("%s does not exist.\n", inputfile);
        return 1;
    }

    // Read header
    // TODO #3
    //-- use WAVHEADER struct from "wav.h" to read bytes 0-43;
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, inpointer);

    // Use check_format to ensure WAV format (at bottom of code)
    // TODO #4
    //-- if WAVHEADER is header, return true;
    //-- bytes 8-11 spell W A V E;
    if (check_format(header) == 0)
    {
        printf("%s is not a wav file.\n", inputfile);
        return 1;
    }

    // Open output file for writing
    // TODO #5
    //-- open write file argv[2];
    char *outputfile = argv[2];
    FILE *outpointer = fopen(outputfile, "wb");
    if (outpointer == NULL)
    {
        printf("Could not create %s.\n", outputfile);
        return 1;
    }

    // Write header to file
    // TODO #6
    //-- copy the header from (TODO #3) to the output file;
    fwrite(&header, sizeof(WAVHEADER), 1, outpointer);

    // Use get_block_size to calculate size of block (at bottom of code)
    //-- HINT: Notice that one of the members of WAVHEADER is bitsPerSample. But to calculate block size, you’ll need bytes per
    // sample!
    // TODO #7
    int blocksize = get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    //-- read the last part and write it as the first part of the output
    //-- "fseek" and "ftell" from the hints
    if (fseek(inpointer, blocksize, SEEK_END))
    {
        return 1;
    }
    BYTE buffer[blocksize]; //--array of size "block size"
    while (ftell(inpointer) - blocksize >
           sizeof(header)) //-- working backwards towards the header in blocks and verifying not too far
    {
        if (fseek(inpointer, -2 * blocksize, SEEK_CUR))
        {
            return 1;
        }
        fread(buffer, blocksize, 1, inpointer);
        fwrite(buffer, blocksize, 1, outpointer);
    }

    fclose(outpointer);
    fclose(inpointer);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    //-- if WAVHEADER is header, return true;
    //-- bytes 8-11 spell W A V E;
    // if (header.format = WAVE)
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1;
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    // Use get_block_size to calculate size of block (at bottom of code)
    //-- For audio, we calculate the size of each block with the following calculation: number of channels multiplied by bytes per
    // sample.
    //-- HINT: Notice that one of the members of WAVHEADER is bitsPerSample. But to calculate block size, you’ll need bytes per
    // sample!
    int bytesize = (header.numChannels * header.bitsPerSample) / 8;
    return bytesize;
}
