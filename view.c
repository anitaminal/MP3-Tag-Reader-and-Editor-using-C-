#include "process.h"
#include "data.h"
#include "edit_process.h"
Status validate_view_args(char *argv[], detail *mem_det)
{   
    //validate the file extension
    if(strstr(argv[2], ".mp3")== NULL)
    {
        printf("\nInfo: Input file format '.mp3' not available\n");
        return e_failure;
    }
    else
    {
        if(strcmp(strstr(argv[2], ".mp3"), ".mp3") == 0)
        {
         //   printf("Info: Input file format .mp3 is available\n");
            mem_det->mp3_fname = argv[2];
        }
        else
        {
            printf("Error: Input file format .mp3 not available\n");
            return e_failure;
        }
    }
    return e_success;
}

Status open_files(detail *mem_det)
{
    //open the file in read mode
    mem_det->fptr_mp3 = fopen(mem_det->mp3_fname, "r");
    if(mem_det->fptr_mp3 == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", mem_det->mp3_fname);
        return e_failure;
    }
    return e_success;
}
Status check_id3_tag(detail *mem_det)
{
     char tag[4];
     char ch;
     char version[3];
    //read the first 3 bytes : ID3 from the mp3 file
    fread(tag, 3, 1, mem_det->fptr_mp3);
    tag[3] = '\0';
    if(strcmp(tag, "ID3") == 0)
    {
     //   printf("Info:ID3 tag is verified successfully\n");
        //validate version : 30
        fread(version, 2, 1, mem_det->fptr_mp3);
       
        if(version[0]== 3 && version[1] == 0)
        {
          //  printf("Info: version is verified successfully\n");
            
        }else
        {
            printf("Error: Version is not verified\n");
            return e_failure;
        }
        
    }
    else
    {
        printf("Error: ID3 is not found in the mp3 file\n");
        return e_failure;
    }
    return e_success;
}
 Status view_title(detail *mem_det)
 {
    char tag[5];
    char ch;
    char data[100];
 // printf("The fetched data is \n");
 //fetch each character and store it in the tag
            int i= 0;
            for(i =0 ;i<4; i++)
            {
                ch = fgetc(mem_det->fptr_mp3);
                tag[i] = ch;
           //   printf("%c", ch );
           //   printf(" %c\n", tag[i]);
            }
            tag[i] = '\0';
           if(strcmp(tag, "TIT2") == 0)
           {
       //     printf("TIT2 TAG IS FOUND\n");
              unsigned int frame_size = 0;
              int i;
            //fetch the size of the detail: size has 4 bytes
                for(i = 0; i < 4; i++)
                {
                    int ch = fgetc(mem_det->fptr_mp3);

                    if(ch == EOF)
                    {
                        return e_failure;
                    }

                    frame_size = (frame_size << 8) | (unsigned char)ch;
                }
                mem_det->title_size = frame_size;
        //      printf("Frame_size = %d %d\n", frame_size, mem_det->title_size);
                // Skip two flag bytes 
                fseek(mem_det->fptr_mp3, 2, SEEK_CUR);

                // Skip one encoding byte 
                fseek(mem_det->fptr_mp3, 1, SEEK_CUR);

                // Read title text and copy it to structure
                int title_size = frame_size - 1;
             
                if(title_size >= 0 && title_size < sizeof(data))
                {
                    fread(data, 1, title_size, mem_det->fptr_mp3);
                    data[title_size] = '\0';

            //      printf("Song title: %s\n", data);
                    strcpy(mem_det->title, data);
            //      printf("Song: %s\n", mem_det->title );
                }
                
           }
           else
           {
            //  printf("TIT2 Tag is not found\n");
                return e_failure;
           }
           return e_success;
 }

Status view_author(detail *mem_det)
{
    char tag[5];
    char ch;
    char data[100];
//  printf("The fetched data is \n");
//read the tag: fetch 4 characters from the file
            int i= 0;
            for(i =0 ;i<4; i++)
            {
                ch = fgetc(mem_det->fptr_mp3);
                tag[i] = ch;
             // printf("%c", ch );
             // printf(" %c\n", tag[i]);
            }
            tag[i] = '\0';
            //check the fetched character is "TPE1"
           if(strcmp(tag, "TPE1") == 0)
           {
        //    printf("TPE1 TAG IS FOUND\n");
              unsigned int frame_size = 0;
              int i;
            //extract the size of the author_name: fetch 4 bytes from the file 
                for(i = 0; i < 4; i++)
                {
                    int ch = fgetc(mem_det->fptr_mp3);

                    if(ch == EOF)
                    {
                        return e_failure;
                    }
                    //update frame_size i.e, size of the detail stored
                    frame_size = (frame_size << 8) | (unsigned char)ch;
                }
    //          printf("Frame_size = %d\n", frame_size);
                // Skip two flag bytes 
                fseek(mem_det->fptr_mp3, 2, SEEK_CUR);

                // Skip one encoding byte 
                fseek(mem_det->fptr_mp3, 1, SEEK_CUR);

                //Read aritist name from file and copy it to structure
                int title_size = frame_size - 1;
                mem_det->artist_size = title_size;
                if(title_size >= 0 && title_size < sizeof(data))
                {
                    fread(data, 1, title_size, mem_det->fptr_mp3);
                    data[title_size] = '\0';

            //      printf("Author Name: %s\n", data);
                    strcpy(mem_det->artist, data);
            //      printf("Author Name: %s\n", mem_det->artist);
                }
                
             } else
           {
            //  printf("TPE1 Tag is not found\n");
                return e_failure;
           }
           return e_success;
}
Status view_album(detail *mem_det)
{
    char tag[4];
    char ch;
    char data[100];
//  printf("The fetched data is \n");
//read the tag: fetch 4 characters from the file
            int i= 0;
            for(i =0 ;i<4; i++)
            {
                ch = fgetc(mem_det->fptr_mp3);
                tag[i] = ch;
        //      printf("%c", ch );
        //      printf(" %c\n", tag[i]);
            }
            tag[i] = '\0';
            //check the fetched character is "TALB"
           if(strcmp(tag, "TALB") == 0)
           {
        //    printf("TALB TAG IS FOUND\n");
              unsigned int frame_size = 0;
              int i;
                 //extract the size of the author_name: fetch 4 bytes from the file 
                for(i = 0; i < 4; i++)
                {
                    int ch = fgetc(mem_det->fptr_mp3);

                    if(ch == EOF)
                    {
                        return e_failure;
                    }
                    //update frame_size i.e, size of the detail stored
                    frame_size = (frame_size << 8) | (unsigned char)ch;
                }
          //    printf("Frame_size = %d\n", frame_size);
                // Skip two flag bytes 
                fseek(mem_det->fptr_mp3, 2, SEEK_CUR);

                // Skip one encoding byte 
                fseek(mem_det->fptr_mp3, 1, SEEK_CUR);

                //read album_name from the file and store it to structure
                int title_size = frame_size - 1;
                mem_det->album_size = title_size;

                if(title_size >= 0 && title_size < sizeof(data))
                {
                    fread(data, 1, title_size, mem_det->fptr_mp3);
                    data[title_size] = '\0';

         //         printf("ALBUM Name: %s\n", data);
                    strcpy(mem_det->album, data);
                }
                
                }else
                {
         //         printf("TALB Tag is not found\n");
                    return e_failure;
                }    
           return e_success;
}
Status view_year(detail *mem_det)
{
    char tag[4];
    char ch;
    char data[100];
 // printf("The fetched data is \n");
 // check the tag: fetch the 4 bytes of the file pointer
            int i= 0;
            for(i =0 ;i<4; i++)
            {
                ch = fgetc(mem_det->fptr_mp3);
                tag[i] = ch;
        //      printf("%c", ch );
        //      printf(" %c\n", tag[i]);
            }
            tag[i] = '\0';
            //compare the tag with "TYER"
           if(strcmp(tag, "TYER") == 0)
           {
        //    printf("TYER TAG IS FOUND\n");
              unsigned int frame_size = 0;
              int i;
            //fetch the size of the year : 4 bytes 
                for(i = 0; i < 4; i++)
                {
                    int ch = fgetc(mem_det->fptr_mp3);

                    if(ch == EOF)
                    {
                        return e_failure;
                    }

                    frame_size = (frame_size << 8) | (unsigned char)ch;
                }
         //     printf("Frame_size = %d\n", frame_size);
                // Skip two flag bytes 
                fseek(mem_det->fptr_mp3, 2, SEEK_CUR);

                // Skip one encoding byte 
                fseek(mem_det->fptr_mp3, 1, SEEK_CUR);

                //read the year from the file and store it to structure
                int title_size = frame_size - 1;
                mem_det->year_size = title_size;

                if(title_size >= 0 && title_size < sizeof(data))
                {
                    fread(data, 1, title_size, mem_det->fptr_mp3);
                    data[title_size] = '\0';

          //        printf("Year: %s\n", data);
                    strcpy(mem_det->year, data);
                }
                
                }else
                {
          //        printf("TYER Tag is not found\n");
                    return e_failure;
                }    
           return e_success;
}
Status view_content(detail *mem_det)
{
    char tag[4];
    char ch;
    char data[100];
//  printf("The fetched data is \n");
//read the tag: fetch the four bytes of the file
            int i= 0;
            for(i =0 ;i<4; i++)
            {
                ch = fgetc(mem_det->fptr_mp3);
                tag[i] = ch;
  //            printf("%c", ch );
  //            printf(" %c\n", tag[i]);
            }
            tag[i] = '\0';
            //compare the tag with TCON
           if(strcmp(tag, "TCON") == 0)
           {
  //          printf("TCON TAG IS FOUND\n");
              unsigned int frame_size = 0;
              int i;
                //fetch the size of the music : read the 4 bytes and update to frame_size
                for(i = 0; i < 4; i++)
                {
                    int ch = fgetc(mem_det->fptr_mp3);

                    if(ch == EOF)
                    {
                        return e_failure;
                    }

                    frame_size = (frame_size << 8) | (unsigned char)ch;
                }
  //            printf("Frame_size = %d\n", frame_size);
                // Skip two flag bytes 
                fseek(mem_det->fptr_mp3, 2, SEEK_CUR);

                // Skip one encoding byte 
                fseek(mem_det->fptr_mp3, 1, SEEK_CUR);

                // Read music details 
                int title_size = frame_size - 1;
                mem_det->music_size = title_size;

                if(title_size >= 0 && title_size < sizeof(data))
                {
                    fread(data, 1, title_size, mem_det->fptr_mp3);
                    data[title_size] = '\0';
                                                                          
         //         printf("Music: %s\n", data);
                    strcpy(mem_det->music, data);
                }
                
                }else
                {
         //         printf("TCON Tag is not found\n");
                    return e_failure;
                }    
           return e_success;
}
Status view_comment(detail *mem_det)
{
    char tag[4];
    char ch;
    char data[100];
 // printf("The fetched data is \n");
 //read the tag: 4 bytes of the file pointer
            int i= 0;
            for(i =0 ;i<4; i++)
            {
                ch = fgetc(mem_det->fptr_mp3);
                tag[i] = ch;
         //     printf("%c", ch );
         //     printf(" %c\n", tag[i]);
            }
            tag[i] = '\0';
            //compare the tag with COMM
           if(strcmp(tag, "COMM") == 0)
           {
        //    printf("COMM TAG IS FOUND\n");
              unsigned int frame_size = 0;
              int i;
                //read tge size of the comment
                for(i = 0; i < 4; i++)
                {
                    int ch = fgetc(mem_det->fptr_mp3);

                    if(ch == EOF)
                    {
                        return e_failure;
                    }
                    //update the frame_size
                    frame_size = (frame_size << 8) | (unsigned char)ch;
                }
         //     printf("Frame_size = %d\n", frame_size);
                // Skip two flag bytes 
                fseek(mem_det->fptr_mp3, 2, SEEK_CUR);

                // Skip one encoding byte 
                fseek(mem_det->fptr_mp3, 1, SEEK_CUR);

                //read the comment and the copy it to structure
                int title_size = frame_size - 1;
                mem_det->comment_size = title_size;

                if(title_size >= 0 && title_size < sizeof(data))
                {
                    fread(data, 1, title_size, mem_det->fptr_mp3);
                    data[title_size] = '\0';

         //         printf("Comment: %s\n", data);
                    strcpy(mem_det->comment, data);
                }
                
                }else
                {
          //        printf("COMM Tag is not found\n");
                    return e_failure;
                }    
           return e_success;
}
Status view_file(char *argv[], detail *mem_det)
{
    char ch;
    char data[100];
    //open the file in read mode
    if(open_files(mem_det)==e_success)
    {
      //  printf("Info: Files are opened successfully\n");
        //verify the document is id3 version 3.0
        if(check_id3_tag(mem_det) == e_success)
        {
            
            rewind(mem_det->fptr_mp3);
          //  printf("Info: ID3 TAG Verified successfully\n");
            //move forward 10bytes :header information
            fseek(mem_det->fptr_mp3, 10, SEEK_SET);
            //fetch the title 
            view_title(mem_det);
            //fetch the author_name
            view_author(mem_det);
            //fetch the album_name
            view_album(mem_det);
            //fetch the year of song
            view_year(mem_det);
            //fetch the content
            view_content(mem_det);
            //fetch the comment
            view_comment(mem_det);
            //display the read details
            display_contents(mem_det);
           
          
        }
    }
    else
    {
        printf("Error: File is not opened successfully\n");
        return e_failure;
    }
    return e_success;
    
}
Status display_contents(detail *mem_det)
{
    print_space();
    printf("MP3 TAG READER AND EDITOR FOR ID3V2\n");
    print_bar();
    printf("%-20s: %s\n","Title", mem_det->title);
    printf("%-20s: %s\n","ARTIST", mem_det->artist);
    printf("%-20s: %s\n", "ALBUM", mem_det->album);
    printf("%-20s: %s\n", "YEAR", mem_det->year);
    printf("%-20s: %s\n", "MUSIC", mem_det->music);     
    printf("%-20s: %s\n", "COMMENT", mem_det->comment);
    
}
