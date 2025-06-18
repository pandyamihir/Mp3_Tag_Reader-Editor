#ifndef TAG_EDIT_H
#define TAG_EDIT_H

#include "main.h"
#include <stdio.h>
#include <stdlib.h>

// Structure to store information for editing an MP3 tag and file pointers
typedef struct
{
    char fname[MAX_TITLE_SIZE];         // File name to be edited
    char edit_content[MAX_TITLE_SIZE];  // Content to be edited in the MP3 tag
    FILE *fptr_edit;                    // File pointer for the source MP3 file to be edited
    FILE *fptr_new;                     // File pointer for the new edited MP3 file
} tag_edit;
    
// Function to validate the input parameters for the edit operation
Status validate(char *argv[], tag_edit *edit_fileinfo);

// Function to open the MP3 file for editing
Status opening_file(tag_edit *edit_fileinfo);

// Function to skip over data in the source file when copying to the destination
Status skip_data(FILE *src, FILE *dest);

// Function to edit the data of a specific tag in the MP3 file
Status edit_data(FILE *src, FILE *dest, char *argv[]);

// Function to handle the user's selected edit operation (e.g., edit title, artist, etc.)
Status edit_option(char *argv[], tag_edit *edit_fileinfo);

// Function to copy the remaining data from the source file to the destination file
Status copy_remaining(FILE *src, FILE *dest);

#endif