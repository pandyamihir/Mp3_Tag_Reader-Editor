#ifndef TYPES_H
#define TYPES_H

// Maximum sizes for various string fields
#define MAX_TITLE_SIZE 255
#define MAX_ARTIST_SIZE 255
#define MAX_ALBUM_SIZE 255
#define MAX_YEAR_SIZE 4
#define MAX_COMMENT_SIZE 255
#define MAX_GENRE_SIZE 50

// Type alias for unsigned int
typedef unsigned int uint;

// Enum to represent operation result status
typedef enum
{
    t_success,  // Operation was successful
    t_failure   // Operation failed
} Status;

// Enum to represent different operation types in the application
typedef enum
{
    t_view,      // View operation
    t_edit,      // Edit operation
    t_help,      // Help operation
    t_unsupported // Unsupported operation
} OperationType;

// Function declaration for displaying errors
void display_error();

#endif
