#include <string.h>
#include <stdio.h>
#include "tag_edit.h"
#include "main.h"

int count=0,flag=1;
char ch;

// Function to validate input arguments and ensure the correct file extension (.mp3)
Status validate(char *argv[], tag_edit * edit_fileinfo)
{
    char *ext = strstr(argv[3], ".");

    if (ext != NULL && strcmp(ext, ".mp3") == 0)
    {
        // Copy file name and content to struct fields
        strcpy(edit_fileinfo->fname, argv[3]);
        strcpy(edit_fileinfo->edit_content, argv[4]);
        
        return t_success;
    }
    else
    {
        return t_failure;
    }
}

// Function to open the original MP3 file for reading and a new file for writing edited content
Status opening_file(tag_edit *edit_fileinfo)
{
    edit_fileinfo -> fptr_edit = fopen(edit_fileinfo -> fname, "r+");

    if(edit_fileinfo -> fptr_edit == NULL)
    {
        return t_failure;
    }

    edit_fileinfo -> fptr_new = fopen("edited_song.mp3", "w+");

    if(edit_fileinfo->fptr_new != NULL)
    {
        return t_success;
    }
    else
    {
        return t_failure;
    }
}

// Function to skip the metadata tag (ID3) and copy remaining data from source to destination
Status skip_data(FILE *src, FILE *dest)
{
    long cur = ftell(src);
    int tag_size=0, count=0;
    char tag[50], tit2_id[4];

    unsigned char size[4];

    // Read and verify the TIT2 frame header
    if(fread(tit2_id, 1, 4, src) != 4)
    {
        return t_failure;
    }

    // Read size of the ID3 tag and verify
    if(fread(size, 1, 4, src) != 4)
    {
        return t_failure;
    }

    // Calculate tag size
    tag_size = (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];

    // Skip the 3-byte header and verify successful seek
    if(fseek(src, 3, SEEK_CUR) != 0)
    {
        return t_failure;
    }

    // Ensure tag size is reasonable
    if (tag_size - 1 > sizeof(tag))
    {
        return t_failure;
    }

    // Read the tag data
    if(fread(tag, 1, tag_size - 1, src) != tag_size - 1)
    {
        return t_failure;
    }

    long end = ftell(src);

    // Seek back to original position to continue copying
    if(fseek(src, cur, SEEK_SET) != 0)
    {
        return t_failure;
    }

    long bytes = (end - cur);

    // Copy remaining data from source to destination
    while (bytes-- > 0)
    {
        fread(&ch, 1, 1, src);
        fwrite(&ch, 1, 1, dest);
    }

    return t_success;
}

// Function to edit the ID3 tag with the new data (e.g., title, artist, etc.)
Status edit_data(FILE *src, FILE *dest, char *argv[])
{
    char tit2_id[4];
    char tsize[4];
    int size = strlen(argv[4]) + 1;

    // Read and verify the TIT2 frame header
    if(fread(tit2_id, 1, 4, src) != 4)
    {
        return t_failure;
    }

    // Write the TIT2 frame header to the new file
    if(fwrite(tit2_id, 1, 4, dest) != 4)
    {
        return t_failure;
    }

    // Read the current tag size
    if(fread(tsize, 1, 4, src) != 4)
    {
        return t_failure;
    }

    int tag_size = (tsize[0] << 21) | (tsize[1] << 14) | (tsize[2] << 7) | tsize[3];

    // Seek back to modify the tag size
    fseek(src, -4, SEEK_SET);

    {
        char frame_size[4];

        // Update the frame size based on the new content
        frame_size[0] = (size >> 24) & 0xFF;
        frame_size[1] = (size >> 16) & 0xFF;
        frame_size[2] = (size >> 8) & 0xFF;
        frame_size[3] =  size & 0xFF;

        // Write the new frame size
        if(fwrite(frame_size, 1, 4, dest) != 4)
        {
            return t_failure;
        }

        // Skip the 3-byte header after the size
        while (count++ < 3)
        {
            fread(&ch, 1, 1, src);
            fwrite(&ch, 1, 1, dest);
        }

        // Write the new content (e.g., title, artist, etc.)
        if(fwrite(argv[4], 1, strlen(argv[4]), dest) != strlen(argv[4]))
        {
            return t_failure;
        }
    }

    // Skip the rest of the old tag data and seek to the new position
    if(fseek(src, tag_size - 1, SEEK_CUR) != 0)
    {
        return t_failure;
    }

    // Print success message
    printf("----------------------------------------------------------------------\n");
    printf("%51s\n","MP3 Tag Data Edited Successfully");
    printf("----------------------------------------------------------------------\n");

    return t_success;
}

// Function to edit different metadata options (-t, -a, -A, -y, -m, -c) based on input arguments
Status edit_option(char *argv[], tag_edit *edit_fileinfo)
{
    // Skip initial data in the MP3 file (before tag information)
    while (ftell(edit_fileinfo->fptr_edit) != 10)
    {
        fread(&ch, 1, 1, edit_fileinfo->fptr_edit);
        fwrite(&ch, 1, 1, edit_fileinfo->fptr_new);
    }

    // Handle various options like title (-t), artist (-a), album (-A), year (-y), genre (-m), and comments (-c)
    if(strcmp(argv[2],"-t") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == t_failure)
        {
            return t_failure;
        }
        
        printf("Title      :  %s\n", argv[4]);
        flag = 0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new) == t_failure)
        {
            return t_failure;
        }
    }
    
    if(strcmp(argv[2],"-a") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == t_failure)
        {
            return t_failure;
        }
        printf("Artist     :  %s\n", argv[4]);
        flag = 0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new) == t_failure)
        {
            return t_failure;
        }
    }
    
    if(strcmp(argv[2],"-A") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == t_failure)
        {
            return t_failure;
        }
        printf("Album     :  %s\n", argv[4]);
        flag = 0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new) == t_failure)
        {
            return t_failure;
        }
    }

    if(strcmp(argv[2],"-y") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == t_failure)
        {
            return t_failure;
        }
        printf("Year     :  %s\n", argv[4]);
        flag = 0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new) == t_failure)
        {
            return t_failure;
        }
    }

    if(strcmp(argv[2],"-m") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == t_failure)
        {
            return t_failure;
        }
        printf("Genre     :  %s\n", argv[4]);
        flag = 0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new) == t_failure)
        {
            return t_failure;
        }
    }

    if(strcmp(argv[2],"-c") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == t_failure)
        {
            return t_failure;
        }
        printf("Comments     :  %s\n", argv[4]);
        flag = 0;
    }
    
    // If no valid flag is set, print an error and usage
    if(flag)
    {
        printf("\nError: Enter a valid edit option\n");
        flag = 1;
        printf("USAGE:\n"
            "To edit please pass like: ./a.out -t/-a/-A/-m/-y/-c <changing text> <mp3_filename>\n");
        return t_failure;
    }

    // Copy remaining content after editing
    if(copy_remaining(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new) == t_failure)
    {
        return t_failure;
    }
    flag = 1;
    
    return t_success;
}

// Function to copy remaining data after editing
Status copy_remaining(FILE * src, FILE* dest)
{
    // Copy all remaining bytes from the source to the destination
    while (fread(&ch, 1, 1, src))
    {
        fwrite(&ch, 1, 1, dest);
    }
    printf("----------------------------------------------------------------------\n");

    return t_success;
}