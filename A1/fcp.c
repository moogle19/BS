#include "fcp.h"

int main(int argc, char **argv)
{
    int readFile;
    int writeFile;

    //checks if number of arguments is correct
    if(argc != 3)
    {
        printf("False number of Arguments!\n");
        return -1; //exit program
    }

    //opens file to copy
    readFile = open(argv[1], O_RDONLY);

    //checks if fopen was succesful
    if(readFile < 0)
    {
        printf("Failed to open file! \n");
        return -1; //exit program
    }

    //creates destionation file
    writeFile = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);

    //checks if output file was created
    if(writeFile < 0)
    {
        printf("Can't create output file! \n");
        return -1; //exit program
    }


    MD5_CTX c;

    int* input = malloc(BUFSIZE);

    unsigned char hash[HASHLENGTH];

    MD5_Init(&c);

    int count = 0;
    int i = 0;

    count = read(readFile, input, sizeof(input));
    //read until end of file
    while(count)
    {   
        MD5_Update(&c, input, count);
        write(writeFile, input, count);
        count = read(readFile, input, sizeof(input));
    }

    MD5_Final(hash ,&c);

    //print original hash
    printf("%s\t", "Original-Hash: ");
    for(i = 0;i < HASHLENGTH*sizeof(unsigned char); i+=sizeof(unsigned char))
    {
        printf("%02x", hash[i]); //fuehrende nullen mit %02 erzwingen
    }
    printf("\n");

    //fclose write and read
    close(writeFile);
    close(readFile);


    //check copyied file
    int readWritten;
    MD5_CTX check;

    unsigned char controlhash[HASHLENGTH];

    MD5_Init(&check);

    readWritten = open(argv[2], O_RDONLY);

    if(readWritten < 0)
    {
        printf("%s\n", "Failed to open");
    }

    count = 0;
    int* newinput = malloc(BUFSIZE);
    count = read(readWritten, newinput, sizeof(newinput));

    while(count)
    {
            MD5_Update(&check, newinput, count);
            count = read(readWritten, newinput, sizeof(newinput));
    }

    MD5_Final(controlhash, &check);

    int hashcheck = 1;

    //print copy hash
    printf("%s\t", "Copy-Hash: ");
    for(i = 0;i < HASHLENGTH*sizeof(unsigned char); i+=sizeof(unsigned char))
    {
        printf("%02x", controlhash[i]); //fuehrende nullen mit %02 erzwingen
        if(hash[i] != controlhash[i])
        {
            hashcheck = 0;
        }
    }
    printf("\n");

    //close
    close(readWritten);

    //free all the allocated space
    free(input);
    free(newinput);

    //output success or failure
    if(hashcheck)
    {
        printf("%s\n", "MD5-Check:\tsuccesful\nCopying:\tsuccesful");
    }
    else
    {
        printf("%s\n", "MD5-Check:\tfailed\nCopying:\tfailed");
    }

    return 0;
}
