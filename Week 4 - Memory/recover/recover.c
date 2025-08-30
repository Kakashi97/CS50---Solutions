#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        return 1;
    }

    uint8_t buffer[512];
    int filecount = 0;
    char *filename = malloc(sizeof(char) * 8);

    if (filename == NULL)
    {
        return 1;
    }

    sprintf(filename, "%03i.jpg", filecount);

    FILE *img = fopen(filename, "w");
    if (img == NULL)
    {
        fclose(card);
        return 1;
    }

    while (fread(buffer, 512, 1, card))
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (filecount == 0)
            {
                fwrite(buffer, 512, 1, img);
                filecount++;
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", filecount);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    return 1;
                }
                fwrite(buffer, 512, 1, img);
                filecount++;
            }
        }
        else
        {
            if (filecount == 0)
                continue;
            else
            {
                fwrite(buffer, sizeof(uint8_t), 512, img);
            }
        }
    }
    free(filename);
    fclose(img);
    fclose(card);
}
