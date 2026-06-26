#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3.h"

// Validates command line arguments  
int check_validation_edit(int argc, char *argv[], mp3_t *ptr)
{
    if (argc != 5)
    {
        return FAILURE;
    }

    if (strcmp(argv[2], "-t") == 0)
        strcpy(ptr->tag, "TIT2"); // Title
    else if (strcmp(argv[2], "-a") == 0)
        strcpy(ptr->tag, "TPE1"); // Artist
    else if (strcmp(argv[2], "-A") == 0)
        strcpy(ptr->tag, "TALB"); // Album
    else if (strcmp(argv[2], "-y") == 0)
        strcpy(ptr->tag, "TYER"); // Year
    else if (strcmp(argv[2], "-m") == 0)
        strcpy(ptr->tag, "TCON"); // Content type/Genre
    else if (strcmp(argv[2], "-c") == 0)
        strcpy(ptr->tag, "COMM"); // Comments
    else
    {
        printf("Unsupported edit option\n");
        return FAILURE;
    }

    // Open file to check for ID3 signature
    ptr->S_fp = fopen(argv[4], "rb");
    if (!ptr->S_fp)
    {
        perror("File open failed");
        return FAILURE;
    }

    // Read first 3 bytes to verify "ID3" header
    char tag_check[4];
    fread(tag_check, 1, 3, ptr->S_fp);
    tag_check[3] = '\0';

    if (strcmp(tag_check, "ID3") != 0)
    {
        printf("Error: Not a valid MP3 file\n");
        fclose(ptr->S_fp);
        return FAILURE;
    }

    // Store the new text content from arguments
    strcpy(ptr->new_content, argv[3]);

    fclose(ptr->S_fp);
    return SUCCESS;
}

// Performs the actual editing by copying data 
int do_edit(int argc, char *argv[], mp3_t *ptr)
{
    // Open source for reading and temp file for writing
    ptr->S_fp = fopen(argv[4], "rb");
    ptr->D_fp = fopen("temp.mp3", "wb");

    if (!ptr->S_fp || !ptr->D_fp)
        return FAILURE;

    // Copy the first 10 bytes (ID3 Header)
    if (copy_header(ptr) == FAILURE)
        return FAILURE;

    char tag[5];
    char flag[2];

    for (int i = 0; i < 6; i++)
    {
        int size;

        // Read and write the Frame ID (4 bytes)
        fread(tag, 1, 4, ptr->S_fp);
        tag[4] = '\0';
        fwrite(tag, 1, 4, ptr->D_fp);

        // Read size and convert from Big Endian to Host order
        fread(&size, 4, 1, ptr->S_fp);
        size = conversion(size, 4);

        // Read Frame Flags (2 bytes)
        fread(flag, 2, 1, ptr->S_fp);

        // If this is the tag we want to change
        if (strcmp(tag, ptr->tag) == 0)
        {
            int new_size = strlen(ptr->new_content) + 1; 
            int be_size = conversion(new_size, 4);       

            fwrite(&be_size, 4, 1, ptr->D_fp); // Write new size
            fwrite(flag, 2, 1, ptr->D_fp);     // Write flags

            fseek(ptr->S_fp, size, SEEK_CUR);

            char *buffer = malloc(new_size);
            buffer[0] = 0x03;
            strcpy(buffer + 1, ptr->new_content);

            fwrite(buffer, 1, new_size, ptr->D_fp);
            free(buffer);
        }
        else
        {
            int be_size = conversion(size, 4);
            fwrite(&be_size, 4, 1, ptr->D_fp);
            fwrite(flag, 2, 1, ptr->D_fp);

            char *content = malloc(size);
            fread(content, 1, size, ptr->S_fp);
            fwrite(content, 1, size, ptr->D_fp);
            free(content);
        }
    }

    // Copy everything after the tags 
    if (copy_remaining_data(ptr) != SUCCESS)
        return FAILURE;

    fclose(ptr->S_fp);
    fclose(ptr->D_fp);

    // Replace original file with the edited temporary file
    remove(argv[4]);
    rename("temp.mp3", argv[4]);

    return SUCCESS;
}

// Copies the initial 10-byte 
int copy_header(mp3_t *ptr)
{
    rewind(ptr->S_fp);
    char buffer[10];
    if (fread(buffer, 1, 10, ptr->S_fp) != 10)
        return FAILURE;
    if (fwrite(buffer, 1, 10, ptr->D_fp) != 10)
        return FAILURE;
    return SUCCESS;
}

// Copies all remaining bytes from source to destination
int copy_remaining_data(mp3_t *ptr)
{
    char ch;
    while (fread(&ch, 1, 1, ptr->S_fp) == 1)
    {
        if (fwrite(&ch, 1, 1, ptr->D_fp) != 1)
            return FAILURE;
    }
    return SUCCESS;
}