#include "process.h"
#include "data.h"
#include "edit_process.h"

int main(int argc, char *argv[])
{
    //validate the command line arguments
    if (argc<2 )
    { 
        print_bar();
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n\n");
        printf("USEAGE:\n1. To view please pass like: \n\n\t\t./a.out -v mp3_file_name\n\n");
        printf("2. To edit please pass like:\n\n\t\t./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3_file_name\n\n3. To get help pass like:\n\n\t\t./a.out --help\n\n");
        
        print_bar();
        return 0;
    }
    //declare the structure variable
    detail mem;


    //check whether view, or edit, or help operation to be performed
    Status ret = check_operation_type(argv);
    {
        if(ret == view)
        {
            //check the file is passed
            if(argv[2] !=NULL)
            {
                if(validate_view_args(argv, &mem)==e_success)
                {
                  //  printf("Read arguments validated successfully\n");
              
                    if(view_file(argv, &mem)==e_success)
                    {
                        print_bar();
                        print_space();
                        printf("SUCCESS: DETAILS DISPLAYED SUCCESSFULLY\n");
                        print_bar();
                    }
                    else
                    {
                        printf("Error: Data cannot be viewed\n");
                        return 0;
                    }
                }
                else
                {
                    printf("ERROR:Argument validation failed\n");
                    return 0;
                }

            }else
            {
                printf("Error: mp3 file is not passed through Command line arguments\n");
                return 0;
            }
        }
        else if(ret == edit)
        {
            //check the file is passed
            if(argc == 5)
            {
                if(validate_edit_args(argv, &mem)==e_success)
                {
                 //   printf("Edit arguments validated successfully\n");
              
                    if(edit_file(argv, &mem)==e_success)
                    {
                        print_bar();
                        print_space();
                        printf("SUCCESS: DETAILS EDITED SUCCESSFULLY\n");
                        print_bar();
                    }
                    else
                    {
                        printf("Error: Data cannot be edited\n");
                        return 0;
                    }
                }
                else
                {
                    printf("ERROR:Edit Argument validation failed\n");
                    return 0;
                }

            } 
            else
            {
                printf("Error: Invalid edit arguments passed\npass:\n./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3_file_name\n");
                return 0;
            }   
        } 
        else if(ret == help)
        {
            print_help();
        }
        else
        {
            printf("Invalid Input\n");
        }
    }
    return 0; 
}