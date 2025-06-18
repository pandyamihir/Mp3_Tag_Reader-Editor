#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "tag_view.h"
#include "tag_edit.h"

// Function to display usage instructions and error messages
void display_error()
{
    printf("Usage:\n");
    printf("To view : ./a.out -v <mp3_filename>\n");
    printf("To help : ./a.out -h\n");
    printf("To edit : ./a.out -e -t/-a/-A/-y/-c/-g <mp3_filename> <new_value>\n");
}

int main(int argc, char *argv[])
{
    // Structures for handling view and edit operations
    tag_view view_fileinfo;
    tag_edit edit_fileinfo;

    // Check if no arguments are provided
    if (argc == 1)
    {
        printf("Error: Invalid Arguments.\n\n");
        display_error();  // Display the usage instructions
        return 1;
    }

    // Check if the user requested help
    if (strcmp(argv[1], "-h") == 0)
    {
        // Display available options and their descriptions
        printf("Modifiers         Functions\n");
        printf("   -v :      View MP3 File Details\n");
        printf("   -h :      Show Help\n");
        printf("   -t :      Modify Title Tag\n");
        printf("   -a :      Modify Artist Tag\n");
        printf("   -A :      Modify Album Tag\n");
        printf("   -y :      Modify Year Tag\n");
        printf("   -c :      Modify Comment Tag\n");
        printf("   -g :      Modify Genre Tag\n");

        return 0;
    }
    else if (strcmp(argv[1], "-v") == 0 && argc == 3) // Handle the view operation
    {
        // Validate file extension and open the file
        if (validate_extn(argv, &view_fileinfo) == t_success && open_file(&view_fileinfo) == t_success)
        {
            // Display the file data if everything is successful
            display_data(&view_fileinfo);
        }
        else
        {
            return t_failure;  // Return failure if validation or file opening fails
        }
    }
    else if (strcmp(argv[1], "-e") == 0 && argc == 5) // Handle the edit operation
    {
        {
            {// Validate the file and extension for editing
                if (validate(argv, &edit_fileinfo) == t_failure)
                {   
                    printf("Invalid file or extension.\n");
                    return 1;  // Return failure if validation fails
                }
            }

            {
                // Try opening the file for editing
                if (opening_file(&edit_fileinfo) == t_failure)
                {
                    printf("Failed to open file for editing.\n");
                    return 1;  // Return failure if the file couldn't be opened
                }
            }

            {// Edit the selected tag
                if(edit_option(argv, &edit_fileinfo) == t_success)
                {
                    if(remove(argv[3]) != 0)
                    {
                        printf("\nError: Failed to remove file\n");
                        return 1;
                    }

                    if(rename("edited_song.mp3",argv[3]) != 0)
                    {
                        printf("\nError: Failed to rename file\n");
                        return 1;
                    }
                }
            }
        }
        // Confirm successful edit
        printf("Tag edited successfully.\n");
    }
    else
    {
        // Display usage instructions if arguments are incorrect
        display_error();
        return 1;
    }

    return 0;
}
