#include "mp3.h"

// Validates the MP3 file and extracts metadata for viewing 
int read_validation_mp3tag(int argc, char *argv[], mp3_t *ptr)
{
    // Check for correct arguments
    if (argc != 3)
    {
        printf("Error : Invalide number of arguments passed\n");
        return FAILURE;
    }

    FILE *fp = fopen(argv[2], "rb");
    if (!fp)
    {
        printf("Erorr : Invalid file name.\n");
        return FAILURE;
    }

    // Check for "ID3" signature at the very beginning of the file
    char mp3name[4];
    fread(mp3name, 1, 3, fp);
    mp3name[3] = '\0';

    if (strcmp(mp3name, "ID3") != 0)
    {
        printf("Erorr : wrong MP3 tag ragar id.\n");
        fclose(fp);
        return FAILURE;
    }

    fseek(fp, 7, SEEK_CUR);

    int size;
    char tag[5];
    char content[100];

    // Read the 6 standard tags (Title, Artist, Album, Year, Genre, Comment)
    for (int i = 0; i < 6; i++)
    {
        // Read Frame ID (e.g., "TIT2")
        fread(tag, 1, 4, fp);
        tag[4] = '\0';
        strcpy(ptr->tag, tag);

        // Read Frame Size and convert from Big Endian to Little Endian
        fread(&size, 1, 4, fp);
        ptr->size = conversion(size, 4);

        // Skip Frame Flags (2 bytes)
        fseek(fp, 2, SEEK_CUR);

        // Read and skip the Encoding Byte (1 byte)
        fgetc(fp);

        // Read actual tag content (size - 1 because we already read the encoding byte)
        fread(content, 1, ptr->size - 1, fp);
        content[ptr->size - 1] = '\0';

        // Store and display the data
        strcpy(ptr->content[i], content);

        if (View_data(ptr, i) == FAILURE)
        {
            printf("Print Meta data failed\n");
            fclose(fp);
            return FAILURE;
        }
    }

    printf("-------------------------------------------------------------------\n");
    fclose(fp);
    return SUCCESS;
}

// Formats and prints the extracted tag data to the console 
int View_data(mp3_t *ptr, int index)
{
    printf("-------------------------------------------------------------------\n");

    if (strcmp(ptr->tag, "TIT2") == 0)
        printf("Title   |\t%-50s|\n", ptr->content[index]);
    else if (strcmp(ptr->tag, "TPE1") == 0)
        printf("Artist  |\t%-50s|\n", ptr->content[index]);
    else if (strcmp(ptr->tag, "TALB") == 0)
        printf("Album   |\t%-50s|\n", ptr->content[index]);
    else if (strcmp(ptr->tag, "TYER") == 0)
        printf("Year    |\t%-50s|\n", ptr->content[index]);
    else if (strcmp(ptr->tag, "TCON") == 0)
        printf("Content |\t%-50s|\n", ptr->content[index]);
    else if (strcmp(ptr->tag, "COMM") == 0)
        printf("Comment |\t%-50s|\n", ptr->content[index]);

    return SUCCESS;
}

// Converts Big Endian integers (MP3 standard) to Little Endian (Host standard) 
int conversion(int num, int size)
{
    char temp;
    char *ptr = (char *)&num;
    for (int i = 0; i < size / 2; i++)
    {
        temp = ptr[i];
        ptr[i] = ptr[size - 1 - i];
        ptr[size - 1 - i] = temp;
    }
    return num;
}