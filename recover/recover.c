#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t  BYTE;

const int BLOCK_SIZE = 512;
const BYTE jpeg[4] = {0xff, 0xd8, 0xff, 0xe0}; // JPEG demarcation
const BYTE jpeg2[4] ={0xff, 0xd8, 0xff, 0xe1};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    char *inputfile = argv[1];
    
    FILE *infile = fopen(inputfile, "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", inputfile);
        return 2;
    }
    
    FILE *img = NULL;
    
    BYTE buf[512];
    int filecounter = 0;
    char filename[8];
   
    // Iterate over file contents
    while(fread(buf, BLOCK_SIZE, 1, infile))
    {
        // Check if the first four bytes are a JPEG signature
        if (memcmp(buf, jpeg, 4) == 0 || memcmp(buf, jpeg2, 4) == 0)
        {
            sprintf(filename, "%03i.jpg", filecounter);
                
            // Open a new JPEG file for writing
            img = fopen(filename, "w");
            
            filecounter++;
        }
        
        if(img)
        {
            fwrite(buf, BLOCK_SIZE, 1, img);
        }

        if(feof(infile))
        {
            fclose(img);
        }
    }
    
    if (img != NULL)
    fclose(img);
    
    fclose(infile);
 
    return 0;
}