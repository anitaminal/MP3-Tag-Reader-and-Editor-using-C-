#ifndef DATA_H
#define DATA_H

#define size 100
typedef struct _details
{
    FILE *fptr_mp3;
    char *mp3_fname;
    int title_size;
    char title[size];
    int artist_size;
    char artist[size];
    int album_size;
    char album[size];
    int year_size;
    char year[size];
    int music_size;
    char music[size];
    int comment_size;
    char comment[size];
    char u_data[size];
    char flag [6];
    
}detail;

typedef enum
{
    e_success,
    e_failure
}Status;

typedef enum
{
    edit,
    view,
    help,
    invalid
}OperationType;

#endif