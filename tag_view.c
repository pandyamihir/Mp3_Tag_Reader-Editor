#include <string.h>
#include <stdlib.h>
#include "tag_view.h"
#include "main.h"

// Function to validate the file extension (checking if it is ".mp3").
Status validate_extn(char *argv[], tag_view *view_fileinfo)
{
    char *ext = strstr(argv[2], ".");  // Search for the file extension.

    // Check if the file extension is ".mp3".
    if (ext != NULL && strcmp(ext, ".mp3") == 0)
    {
        view_fileinfo->fname = argv[2];  // Set the filename if valid.
        return t_success;  // Return success.
    }
    else
    {
        return t_failure;  // Return failure if the extension is not ".mp3".
    }
}

// Function to open the MP3 file in read-write mode.
Status open_file(tag_view *view_fileinfo)
{
    // Open the file specified by `fname` in read-write mode ("r+").
    view_fileinfo->fptr_mp3 = fopen(view_fileinfo->fname, "r+");

    // Check if the file was successfully opened.
    if (view_fileinfo->fptr_mp3 != NULL)
    {
        return t_success;  // Return success.
    }
    else
    {
        return t_failure;  // Return failure if the file could not be opened.
    }
}

// Function to extract data from the MP3 file (metadata).
Status extract_data(FILE *fptr, char *data)
{
    int tag_size = 0;
    char tag[50];  // Buffer to store the extracted tag data.
    unsigned char size[4];  // Buffer to store the size of the tag.

    // Read the 4-byte size to determine the tag size.
    if (fread(size, 1, 4, fptr) != 4)
    {
        return t_failure;  // Return failure if reading the size fails.
    }

    // Calculate the tag size from the 4-byte data.
    tag_size = (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];

    // Move the file pointer to the correct position.
    if (fseek(fptr, 3, SEEK_CUR) != 0)
    {
        return t_failure;  // Return failure if seeking fails.
    }

    // Read the tag data from the file.
    if (fread(tag, 1, tag_size - 1, fptr) != tag_size - 1)
    {
        return t_failure;  // Return failure if reading tag data fails.
    }

    tag[tag_size - 1] = '\0';  // Null-terminate the tag data.

    strcpy(data, tag);  // Copy the tag data to the provided buffer.

    return t_success;  // Return success after extracting the data.
}

// Function to display the MP3 tag data (e.g., title, artist, album, etc.).
Status display_data(tag_view *view_fileinfo)
{
    printf("----------------------------------------------------------------------\n");
    printf("%40s\n", "MP3 Tag Data");
    printf("----------------------------------------------------------------------\n");

    { // Display the Tag ID.
        char buffer[4];
        if (fread(buffer, 1, 3, view_fileinfo->fptr_mp3) != 3)
        {
            return t_failure;  // Return failure if reading the tag ID fails.
        }
        buffer[3] = '\0';  // Null-terminate the string.
        printf("%s ", buffer);  // Print the tag ID.
    }

    { // Display the version (V2).
        unsigned char version[3];
        if (fread(version, 1, 2, view_fileinfo->fptr_mp3) != 2)
        {
            return t_failure;  // Return failure if reading the version fails.
        }
        printf("V2.%d.%d\n", version[0], version[1]);  // Print the version.
    }

    { // Display the Title.
        char tit2_id[5];
        if (fseek(view_fileinfo->fptr_mp3, 5, SEEK_CUR) != 0)
        {
            return t_failure;  // Return failure if seeking fails.
        }
        if (fread(tit2_id, 1, 4, view_fileinfo->fptr_mp3) != 4)
        {
            return t_failure;  // Return failure if reading the title fails.
        }
        tit2_id[4] = '\0';  // Null-terminate the string.

        if (extract_data(view_fileinfo->fptr_mp3, view_fileinfo->title) == t_success)
        {
            printf("Title     :  %s\n", view_fileinfo->title);  // Print the title.
        }
        else
        {
            return t_failure;  // Return failure if extracting the title fails.
        }
    }

    { // Display the Artist.
        char artist_id[5];
        if (fread(artist_id, 1, 4, view_fileinfo->fptr_mp3) != 4)
        {
            return t_failure;  // Return failure if reading the artist fails.
        }
        artist_id[4] = '\0';  // Null-terminate the string.

        if (extract_data(view_fileinfo->fptr_mp3, view_fileinfo->artist) == t_success)
        {
            printf("Artist    :  %s\n", view_fileinfo->artist);  // Print the artist.
        }
        else
        {
            return t_failure;  // Return failure if extracting the artist fails.
        }
    }

    { // Display the Album.
        char album_id[5];
        if (fread(album_id, 1, 4, view_fileinfo->fptr_mp3) != 4)
        {
            return t_failure;  // Return failure if reading the album fails.
        }
        album_id[4] = '\0';  // Null-terminate the string.

        if (extract_data(view_fileinfo->fptr_mp3, view_fileinfo->album) == t_success)
        {
            printf("Album     :  %s\n", view_fileinfo->album);  // Print the album.
        }
        else
        {
            return t_failure;  // Return failure if extracting the album fails.
        }
    }

    { // Display the Year.
        char tyer_id[5];
        if (fread(tyer_id, 1, 4, view_fileinfo->fptr_mp3) != 4)
        {
            return t_failure;  // Return failure if reading the year fails.
        }
        tyer_id[4] = '\0';  // Null-terminate the string.

        if (extract_data(view_fileinfo->fptr_mp3, view_fileinfo->year) == t_success)
        {
            printf("Year      :  %s\n", view_fileinfo->year);  // Print the year.
        }
        else
        {
            return t_failure;  // Return failure if extracting the year fails.
        }
    }

    { // Display the Genre.
        char tcon_id[5];
        if (fread(tcon_id, 1, 4, view_fileinfo->fptr_mp3) != 4)
        {
            return t_failure;  // Return failure if reading the genre fails.
        }
        tcon_id[4] = '\0';  // Null-terminate the string.

        if (extract_data(view_fileinfo->fptr_mp3, view_fileinfo->genre) == t_success)
        {
            printf("Genre     :  %s\n", view_fileinfo->genre);  // Print the genre.
        }
        else
        {
            return t_failure;  // Return failure if extracting the genre fails.
        }
    }

    { // Display the Comments.
        char comm_id[5];
        if (fread(comm_id, 1, 4, view_fileinfo->fptr_mp3) != 4)
        {
            return t_failure;  // Return failure if reading the comments fails.
        }
        comm_id[4] = '\0';  // Null-terminate the string.

        if (extract_data(view_fileinfo->fptr_mp3, view_fileinfo->comm) == t_success)
        {
            printf("Comments  :  %s\n", view_fileinfo->comm);  // Print the comments.
        }
        else
        {
            return t_failure;  // Return failure if extracting the comments fails.
        }
    }

    printf("----------------------------------------------------------------------\n");

    return t_success;  // Return success after displaying all the data.
}
