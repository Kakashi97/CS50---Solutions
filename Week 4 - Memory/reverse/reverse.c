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
    if (argc != 3)
    {
        printf("Usage: %s input.wav output.wav\n", argv[0]);
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("Could not open file.\n");
        fclose(inptr);
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, inptr);

    // Use check_format to ensure WAV format
    // TODO #4
    check_format(header);

    // Open output file for writing
    // TODO #5
    FILE *outptr = fopen(argv[2], "w");
    if (outptr == NULL)
    {
        printf("Could not open file.\n");
        fclose(inptr);
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, outptr);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    // Write reversed audio to file
    // TODO #8

    BYTE buffer[block_size];
    fseek(inptr, 0, SEEK_END);

    while (ftell(inptr) > sizeof(WAVHEADER))
    {
        fseek(inptr, -block_size, SEEK_CUR);
        fread(buffer, block_size, 1, inptr);
        fwrite(buffer, block_size, 1, outptr);
        fseek(inptr, -block_size, SEEK_CUR);
    }

    fclose(inptr);
    fclose(outptr);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != "WAVE"[i])
            return 0;
    }
    return 1;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int bytesPerSample = header.bitsPerSample / 8;
    return header.numChannels * bytesPerSample;
}
