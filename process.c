#include "process.h"
#include "data.h"

void print_bar(void)
{   printf("\n");
    for(int i = 0; i<75; i++)
    {
        printf("-");
    }
    printf("\n");


}
void print_space(void)
{
    for(int i= 0; i<25; i++)
    {
        printf(" ");
    }
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-v") == 0)
    {
        print_bar();
        print_space();
        printf("SELECTED VIEW DETAILS\n");
        print_bar();
        return view;

    }else if(strcmp(argv[1], "-e") == 0)
    {
        print_bar();
        print_space();
        printf("SELECTED EDIT DETAILS\n");
        print_bar();
        return edit;

    }else if(strcmp(argv[1], "--help")==0)
    {
        print_bar();
        print_space();
        printf("HELP MENU SELECTED\n");
        return help;
    }else
    {
        return invalid;
    }
}

void print_help(void)
{
    print_bar();
    printf("1. -v -> to view mp3 file contents\n2. -e -> to edit mp3 file contents\n");
    printf("\t2.1\t -t -> to edit song title\n\t2.2\t -a -> to edit artist name\n\t2.3\t -A -> to edit artist name\n");
    printf("\t2.4\t -y -> to edit year\n\t2.5\t -m -> to edit content\n\t2.6\t -c -> to edit comment\n");
    print_bar();
}