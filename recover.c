#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // Initialize integers
    const int BLOCK_SIZE = 512;
    int jpg_count = 0;

    // Define byte data type
    typedef uint8_t BYTE;

    // Open input file
    FILE *input = fopen(argv[1], "r");

    // Allocate memory for filename array
    char *filename = malloc(9 * sizeof(char));
    sprintf(filename, "%03i.jpg", jpg_count);

    // Allocate memory for JPEG file
    FILE *output = fopen(filename, "w");

    // Create array for buffer to read into
    BYTE buffer[BLOCK_SIZE];

    // Check if raw file inputted
    if (argc != 2)
    {
        printf("Error, no raw file entered.\n");
        return 1;
    }

    // Check if raw file is valid
    if (input == NULL)
    {
        printf("Error, could not open.\n");
        return 1;
    }

    // Keep reading blocks until none are left
    while (fread(&buffer, BLOCK_SIZE, 1, input) == 1)
    {
        // See if header of 512 byte block indicates the start of a new JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Condition for the start of the first JPEG file header
            if (jpg_count == 0)
            {
                fwrite(&buffer, BLOCK_SIZE, 1, output);
                jpg_count++;
            }

            // Condition for each subsequent new JPEG file header
            else
            {
                fclose(output);
                sprintf(filename, "%03i.jpg", jpg_count);
                output = fopen(filename, "w");
                fwrite(&buffer, BLOCK_SIZE, 1, output);
                jpg_count++;
            }
        }

        // Continue writing to previous JPEG file
        else if (jpg_count > 0)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, output);
        }
    }

    // Close files
    free(filename);
    fclose(input);
    fclose(output);
    return 0;
}
