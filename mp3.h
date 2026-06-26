#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1

// Viewing and editing modes */
typedef enum
{
    e_view,
    e_edit,
    e_unsupport,
} opretion_type;

/* Main structure to hold file pointers and tag information */
typedef struct mp3tag
{
    char tag[5];           // Stores the current 4-character Frame ID 
    char fname[50];        // Stores the name of the MP3 file
    int size;              // Stores the size of the current tag content
    FILE *S_fp;            // Source file pointer (original MP3)
    FILE *D_fp;            // Destination file pointer 
    char new_content[100]; // Buffer for the new text being written during an edit
    char content[6][100];  // Array to store the text of the 6 standard ID3v2 tags
} mp3_t;


// Checks if the user entered -v or -e 
opretion_type check_opretion(char *argv[]);

/* Validates the file format and extracts tags into the struct */
int read_validation_mp3tag(int argc, char *argv[], mp3_t *ptr);

/* Utility to handle Big Endian to Little Endian byte swapping */
int conversion(int num, int size);

/* Formats and displays a specific tag's content */
int View_data(mp3_t *ptr, int index);

// --- EDIT FUNCTIONS ---

/* Validates edit arguments and maps user flags (like -t) to Frame IDs */
int check_validation_edit(int argc, char *argv[], mp3_t *ptr);

/* Coordinates the process of copying data and updating specific tags */
int do_edit(int argc, char *argv[], mp3_t *ptr);

/* Copies the initial 10-byte ID3v2 header to the new file */
int copy_header(mp3_t *ptr);

/* Copies the remaining audio data after the tags are processed */
int copy_remaining_data(mp3_t *ptr);