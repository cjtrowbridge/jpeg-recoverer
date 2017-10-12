#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <string.h>


// Function prototypes. Don't change these.
unsigned char * read_card(char * fname, int *size);
void save_jpeg(unsigned char * data, int size, char * fname);
void recover(unsigned char * data, int size);

int main()
{
    // Read the card.raw file into an array of bytes (unsigned char)
    int card_length;
    unsigned char *card = read_card("card.raw", &card_length);
        
    
    // Recover the images
    recover(card, card_length);
}

unsigned char * read_card(char * fname, int * size)
{

    struct stat st;
    if (stat(fname, &st) == -1)
    {
        fprintf(stderr, "Can't get info about %s\n", fname);
        exit(1);
    }
    int len = st.st_size;
    unsigned char *raw = (unsigned char *)malloc(len * sizeof(unsigned char));
    
    FILE *fp = fopen(fname, "rb");
    if (!fp)
    {
        fprintf(stderr, "Can't open %s for reading\n", fname);
        exit(1);
    }
    
    char buf[512];
    int r = 0;
    while (fread(buf, 1, 512, fp))
    {
        for (int i = 0; i < 512; i++)
        {
            raw[r] = buf[i];
            r++;
            
        }
    }
    fclose(fp);
    
    *size = len;
    return raw;
}

void save_jpeg(unsigned char * data, int size, char * fname)
{
    FILE *fp = fopen(fname, "wb");
    if (!fp)
    {
        fprintf(stderr, "Can't write to %s\n", fname);
        exit(1);
    }
    
    fwrite(data, 1, size, fp);
    fclose(fp);   
}



void recover(unsigned char * data, int size){
    
    
    //printf("0x%x\n",data[0]);
    //printf("0x%x\n",data[1]);
    //printf("0x%x\n",data[2]);
    //printf("0x%x\n",data[3]);
    
    printf("Raw File Size: %d\n",size);
    
    int sectorSize = 512;
    int sectorCount = ceil(size/sectorSize);
    int currentSector = 0;
    int jpegCount = 0;
    int jpegStartPosition = 0;

    printf("Sectors In Raw File: %d\n",sectorCount);
    
    
    for(int currentSector = 0; currentSector<sectorCount; currentSector ++){
        
        int filePosition = currentSector * sectorSize;
        
        if(
            (
                data[(filePosition + 0)] == 0xff &&
                data[(filePosition + 1)] == 0xd8 &&
                data[(filePosition + 2)] == 0xff && 
                data[(filePosition + 3)] == 0xe0 
            )||(
                data[(filePosition + 0)] == 0xff &&
                data[(filePosition + 1)] == 0xd8 &&
                data[(filePosition + 2)] == 0xff && 
                data[(filePosition + 3)] == 0xe1
            )||(
                currentSector==sectorCount
            )
        ){
            
            if(jpegStartPosition != 0){
                
                jpegCount++;
                    
                char* fname;
                fname = (char*) malloc (10);
                sprintf(fname,"%d%s",jpegCount,".jpg");
               
                int jpegLength = (filePosition-jpegStartPosition-1);
                
                printf("Saving %s (Position: %d-%d)(Length: %d)\n",fname,jpegStartPosition,(jpegStartPosition+jpegLength),jpegLength);
                
                save_jpeg( &data[jpegStartPosition] , jpegLength, fname);
            
            }
            jpegStartPosition = filePosition;
            
        }
        
    }
    
    printf("Found %d JPEGs\n",jpegCount);
    
}
