#include "fcp.h"

int main(int argc, char **argv)
{
    int readFile;
    int writeFile;

    //checks if number of arguments is correct
    if(argc != 3)
    {
        printf("False number of Arguments!\nUsing: ./fcp <File to Copy> <Destination>\n");
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

    //creates destionation file with write-only and set read/write/execution-rights for user
    writeFile = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);

    //checks if output file was created
    if(writeFile < 0)
    {
        printf("Can't create output file! \n");
        return -1; //exit program
    }


    MD5_CTX c; //init MD5 variable

    int* inputbuffer = malloc(BUFSIZE); //init buffer for copying

    unsigned char hash[HASHLENGTH]; //init variable for hash

    MD5_Init(&c); //init MD5

    int bytes = 0;
    int i = 0;

    //read source file
    bytes = read(readFile, inputbuffer, sizeof(inputbuffer));
    
    //read until no bytes left
    while(bytes)
    {   
        MD5_Update(&c, inputbuffer, bytes); //update md5 hash with current bytes
        write(writeFile, inputbuffer, bytes); //write current bytes
        bytes = read(readFile, inputbuffer, sizeof(inputbuffer)); //read new data
    }

    MD5_Final(hash ,&c); //finalize MD5 Hash

    //close write and read
    close(writeFile);
    close(readFile);

    printf("%s\n", "Done Copying!");

    //print original hash
    printf("%s\t", "Original-Hash: ");
    for(i = 0;i < HASHLENGTH*sizeof(unsigned char); i+=sizeof(unsigned char))
    {
        printf("%02x", hash[i]); //fuehrende nullen mit %02 erzwingen
    }
    printf("\n");
    

    /* Check copyied file */
    int readWritten;

    unsigned char controlhash[HASHLENGTH];

    MD5_Init(&c); //reinit MD5 for write-success test

    readWritten = open(argv[2], O_RDONLY); //open previously written file

    //test if file opened succesful
    if(readWritten < 0)
    {
        printf("%s\n", "Failed to open");
    }

    bytes = 0;
    int* newinputbuffer = malloc(BUFSIZE);
    bytes = read(readWritten, newinputbuffer, sizeof(newinputbuffer));

    while(bytes)
    {
            MD5_Update(&c, newinputbuffer, bytes);
            bytes = read(readWritten, newinputbuffer, sizeof(newinputbuffer));
    }

    //finalize MD5 Hash
    MD5_Final(controlhash, &c);

    int hashcheck = 1; //checkvalue for equality of hashes

    //print copy hash
    printf("%s\t", "Copy-Hash: ");
    for(i = 0;i < HASHLENGTH*sizeof(unsigned char); i+=sizeof(unsigned char))
    {
        printf("%02x", controlhash[i]); //fuehrende nullen mit %02 erzwingen
        //check if hashes are equal
        if(hash[i] != controlhash[i])
        {
            hashcheck = 0;
        }
    }
    printf("\n");

    //close
    close(readWritten);

    //free all the allocated space
    free(inputbuffer);
    free(newinputbuffer);

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
