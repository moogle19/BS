#include "fcp.h"

int main(int argc, char **argv)
{
    int readFile, writeFile, readWritten;
    int* inputbuffer = malloc(BUFSIZE);
    int bytes = 0, i = 0, hashcheck = 1;
    MD5_CTX c; //init MD5 variable
    unsigned char hash[HASHLENGTH], controlhash[HASHLENGTH]; //init variable for hash

    
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

    MD5_Init(&c); //init MD5

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
    
    MD5_Init(&c); //reinit MD5 for write-success test

    readWritten = open(argv[2], O_RDONLY); //open previously written file

    //test if file opened succesful
    if(readWritten < 0)
    {
        printf("%s\n", "Failed to open");
    }
    
    bytes = read(readWritten, inputbuffer, sizeof(inputbuffer));

    while(bytes)
    {
            MD5_Update(&c, inputbuffer, bytes);
            bytes = read(readWritten, inputbuffer, sizeof(inputbuffer));
    }

    //finalize MD5 Hash
    MD5_Final(controlhash, &c);


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
    //free(newinputbuffer);

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