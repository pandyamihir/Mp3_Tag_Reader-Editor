#ifndef view_H
#define view_H

#include "main.h"
#include<stdio.h>

// Defining a structure for holding MP3 tag information (metadata).
typedef struct
{
    char *fname;            // File name (MP3 file name).
    char title[50];         // Title of the song.
    char artist[50];        // Artist name.
    char album[50];         // Album name.
    char year[5];           // Year of release.
    char genre[30];         // Genre of the music.
    char comm[50];          // Any comments related to the song.
    FILE *fptr_mp3;         // Pointer to the MP3 file, used for file handling.
} tag_view;

// Function prototype for checking the type of operation requested (e.g., viewing metadata).
OperationType check_operation_type(char *argv[]);

// Function prototype to validate the file extension (e.g., checks if it's an MP3 file).
Status validate_extn(char *argv[], tag_view *view_fileinfo);

// Function prototype to open the MP3 file for reading metadata.
Status open_file(tag_view *view_fileinfo);

// Function prototype to display the extracted metadata.
Status display_data(tag_view *view_fileinfo);

// Function prototype to extract metadata from the MP3 file.
Status extract_data(FILE* fptr, char *data);

#endif
