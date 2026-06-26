#include<stdio.h>
#include "mp3.h"

int main(int argc, char *argv[])
{
    // Check if the user provided at least one argument 
    if (argc < 2)
    {
        printf("ERROR: INVALID ARGUMENTS\n");
        return FAILURE;
    }

    mp3_t info;
    // Determine if the user wants to view (-v) or edit (-e)
    opretion_type op = check_opretion(argv);

    // Handle View Operation
    if (op == e_view)
    {
        // Validate the file and arguments before reading
        if (read_validation_mp3tag(argc, argv, &info) == SUCCESS)
        {
            printf("View data successfully\n");
            return SUCCESS;
        }
        printf("View data not shown\n");
        return FAILURE;
    }
    // Handle Edit Operation
    else if (op == e_edit)
    {
        // 1. Validate the arguments and the specific tag to be changed
        if (check_validation_edit(argc, argv, &info) == SUCCESS)
        {
            // 2. Perform the actual file modification (copying to temp and renaming)
            if (do_edit(argc, argv, &info) == SUCCESS)
            {
                printf("Edit data is Successful\n");
                return SUCCESS;
            }
        }
        printf("ERROR : Edit data not Successful\n");
        return FAILURE;
    }

    // Fallback for unsupported flags
    printf("Invalid operation\n");
    return FAILURE;
}

opretion_type check_opretion(char *argv[])
{
    // Check for View flag
    if (strcmp(argv[1], "-v") == 0)
    {
        return e_view;
    }
    // Check for Edit flag
    else if (strcmp(argv[1], "-e") == 0)
    {
        return e_edit;
    }
    // Print help menu for invalid or missing flags
    else
    {
        printf("\n-v -> to view mp3 file contents\n");
        printf("-e -> to edit mp3 file contents\n");
        printf("        -t -> to edit song title\n");
        printf("        -a -> to edit artist name\n");
        printf("        -A -> to edit album name\n");
        printf("        -y -> to edit year\n");
        printf("        -m -> to edit content\n");
        printf("        -c -> to edit comment\n");
    }
    return e_unsupport;
}