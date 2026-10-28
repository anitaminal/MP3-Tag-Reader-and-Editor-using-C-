#include "process.h"
#include "edit_process.h"
#include "data.h"

char *target_tag = NULL;
char *field_name = NULL;

Status check_function(char *argv[], detail *mem_det)
{   
     //initialize the flag array to 0 
    for(int i = 0; i<6; i++)
    {
        mem_det->flag[i] = 0;
    }
    if(argv[3] != NULL)
    {
        print_space();
        //based on the argv[2] , make the respective flag to 1 
        //copy the updated detail to u_data of the structure
        //based on the detail to be edited, update the target_tag and field_name
        if(strcmp(argv[2], "-t")== 0)
        {
            printf("CHANGE THE SONG TITLE\n");
            mem_det->flag[0]= 1;
            strcpy(mem_det->title, argv[3]);
            target_tag = "TIT2";
            field_name = "TITLE";
        }        
        else if(strcmp(argv[2], "-a") == 0)
        {
            printf("CHANGE THE ARTIST NAME\n");
            mem_det->flag[1] = 1;
            strcpy(mem_det->artist, argv[3]);
            target_tag = "TPE1";
            field_name = "ARTIST";
        }
        else if(strcmp(argv[2], "-A")==0)
        {
            printf("CHANGE THE ALBUM NAME\n");
            mem_det->flag[2] = 1;
            strcpy(mem_det->album, argv[3]);
            target_tag = "TALB";
            field_name = "ALBUM";
        }
        else if(strcmp(argv[2], "-y") == 0)
        {
            printf("CHANGE THE YEAR\n");
            mem_det->flag[3] = 1;
            strcpy(mem_det->year, argv[3]);
            target_tag = "TYER";
            field_name = "YEAR";
        }
        else if(strcmp(argv[2], "-m")==0)
        {
            printf("CHANGE THE CONTENT\n");
            mem_det->flag[4] = 1;
            strcpy(mem_det->music, argv[3]);
            target_tag = "TCON";
            field_name = "MUSIC";
        }
        else if(strcmp(argv[2], "-c") == 0)
        {
            printf("CHANGE THE COMMENT\n");
            mem_det->flag[5] = 1;
            strcpy(mem_det->comment, argv[3]);
            target_tag = "COMM";
            field_name = "COMMENT";
        }
        strcpy(mem_det->u_data, argv[3]);
        print_bar();
    }
    else
    {
        printf("ERROR: Updated detail are not passed\n");
        return e_failure;
    }
    return e_success;
}
Status validate_edit_args(char *argv[], detail *mem_det)
{
    //check the function to be performed
     //check if argv[2] to determine what to update
     if(argv[2] != NULL)
    {
         if(check_function(argv, mem_det) == e_success)
         {
          //  printf("function passed and updated detail is validated\n");
    
        }else
        {
            printf("ERROR: Function and updated detail are not validated\n");
            return e_failure;
        }  
    }
     else
    {
        printf("ERROR: THE FUNCTION ARGUMENT NOT PASSED\n -t/-a/-A/-m/-y/-c\n");
        return e_failure;
    }
    //validate the file extension
    if(strstr(argv[4], ".mp3")== NULL)
    {
        printf("\nERROR: Input file format '.mp3' not available\n");
        return e_failure;
    }
    else
    {
        if(strcmp(strstr(argv[4], ".mp3"), ".mp3") == 0)
        {
           // printf("Info: Input file format .mp3 is available\n");
           //update the file name to structure mem : mp3_fname
            mem_det->mp3_fname = argv[4];
        }
        else
        {
            printf("Error: Input file format .mp3 not available\n");
            return e_failure;
        }
    }
    return e_success;
}
Status open_edit_files(char *argv[], detail *mem_det)
{
    //open the file in read+ mode
    mem_det->fptr_mp3 = fopen(mem_det->mp3_fname, "r+");
    if(mem_det->fptr_mp3 == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", mem_det->mp3_fname);
        return e_failure;
    }
    return e_success;
}

Status edit_contents(detail *mem_det)
{
    char tag[5];
    unsigned int frame_size;
    int ch;
    //loop runs 6 times to denote each flag index
    for (int i = 0; i < 6; i++)
    {
        // Read tag ID 
        for (int j = 0; j < 4; j++)
        {
            //read a character from the file 
            ch = fgetc(mem_det->fptr_mp3);
            //check if the read character is not end of file
            if (ch == EOF)
                return e_failure;
             //update the tag index
            tag[j] = ch;
        }
        tag[4] = '\0';

        // Read frame size 
        frame_size = 0;
       //read the size of the detail to be edited
        for (int j = 0; j < 4; j++)
        {
            //fetch charcter from the file
            ch = fgetc(mem_det->fptr_mp3);
            //check if the character is not the end of the pointer
            if (ch == EOF)
                return e_failure;
            //update the frame_size: combine each byte to frame_size to obtain size of the detail
            frame_size = (frame_size << 8) | ch;
        }

        // Skip flags 
        
        if (fseek(mem_det->fptr_mp3, 2, SEEK_CUR) != 0)
        {
            return e_failure;
        }
        // Check selected tag 
        if (mem_det->flag[i] == 1 )
        {
            // Skip encoding byte 
            fseek(mem_det->fptr_mp3, 1, SEEK_CUR);
            //find the size of the detail stored in the file
            int old_size = frame_size - 1;
            //find the size of the detail to be edited
            int new_size = strlen(mem_det->u_data);
            //if the size of new_size is greater than the size of the original detail, return failure
            if (new_size > old_size)
            {
                printf("New text must be %d bytes long\n Detail cannot be edited\n",
                       old_size);
                return e_failure;
            }
            if(new_size <=old_size)
            {
                //write the new_detail to the file and validate it 
                if(fwrite(mem_det->u_data,1, new_size,
                mem_det->fptr_mp3)!= new_size)
                {
                    return e_failure;
                }
                //if the new_size is less than the original size then fill the remaining space with space
                for(int i= new_size; i<old_size; i++)
                {
                     if(fputc(' ', mem_det->fptr_mp3)== EOF)
                     return e_failure;
                }
                //notify the user about the detail updated
                printf("%s = %s\n", field_name, mem_det->u_data);
                return e_success;
             
            }
           
        }

        //skip if the remaining frame details 
        fseek(mem_det->fptr_mp3, frame_size, SEEK_CUR);
    }

    return e_failure;
}

Status edit_file(char *argv[], detail *mem_det )
{
    
    char ch;
    char data[100];
    //open the file in read mode
    if(open_edit_files(argv, mem_det)==e_success)
    {
      //  printf("Info: Files are opened successfully\n");
        if(check_id3_tag(mem_det) == e_success)
        { 
            rewind(mem_det->fptr_mp3);
           // printf("Info: ID3 TAG Verified successfully\n");
            fseek(mem_det->fptr_mp3, 10, SEEK_SET);
          
            if(edit_contents(mem_det) == e_failure)
            {
               
                printf("ERROR: Details not edited \n");
                return e_failure;
            }
            
          
        }
    }
    else
    {
        printf("Error: File is not opened successfully\n");
        return e_failure;
    }
    return e_success;
    
}