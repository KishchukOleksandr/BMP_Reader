#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <array>

#define HEADER_SIZE 14
#define INFO_SIZE 124

int main(int argv, const char* argc[])
{
    ssize_t numRead;

    #pragma pack(1)
    union
    {
        char buffer[HEADER_SIZE];
        struct
        {
            char type[2];
            int size;
            short reserved1;
            short reserved2;
            int offsetBits;
        }data;        
    }header;    

    union
    {
        char buffer[INFO_SIZE];
        struct
        {
            unsigned int headerSize;
            unsigned int width;
            unsigned int hight;
            unsigned short int planes;
            unsigned short int bits;
            unsigned int compression;
            unsigned int imageSize;
            unsigned int xResolution;
            unsigned int yResolution;
        }data;        
    }info;  
    
    int fd;

    fd = open("image.bmp", O_RDONLY);

    if(fd != -1)
    {
        numRead = read(fd, header.buffer, HEADER_SIZE);

        if(numRead > 0)
        {            
            std::cout << "file type:\t" << header.data.type[0] << header.data.type[1] << '\n';
            std::cout << "file size:\t" << header.data.size << '\n';
            std::cout << "offset bits:\t" << header.data.offsetBits << '\n';
            
        }

        numRead = read(fd, info.buffer, header.data.offsetBits - HEADER_SIZE);

        if(numRead > 0)
        {            
            std::cout << "header size:\t" << info.data.headerSize << '\n';
            std::cout << "image width:\t" << info.data.width << '\n';
            std::cout << "image hight:\t" << info.data.hight << '\n';
            std::cout << "planes:\t" << info.data.planes << '\n';
            std::cout << "image size:\t" << info.data.imageSize << '\n';
            std::cout << "x res:\t" << info.data.xResolution << '\n';
            std::cout << "y res:\t" << info.data.yResolution << '\n';
        }

        lseek(fd, header.data.offsetBits, SEEK_SET);

        char *image = new char[info.data.imageSize + 1];

        numRead = read(fd, image, info.data.imageSize);
        

        if(numRead > 0)
        {
            for(int i = 0; i < numRead; i++)
            {
                *image++ ^= 0xff;     
            }
        }

        lseek(fd, header.data.offsetBits, SEEK_SET);
        write(fd, image, numRead);
    }
    else
        std::cout << "FILE OPEN ERROR" << '\n';
        
    close(fd);

    return 0;
}