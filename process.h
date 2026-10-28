#ifndef PROCESS_H
#define PROCESS_H

#include<stdio.h>
#include<string.h>
#include "data.h"



void print_bar(void);
void print_space(void);
OperationType check_operation_type(char *argv[]);
void print_help(void);

Status view_file(char *argv[], detail *mem_det);
Status validate_view_args(char *argv[], detail *mem_det);

Status open_files(detail *mem_det);
Status check_id3_tag(detail *mem_det);
Status view_title(detail *mem_det);
Status view_author(detail *mem_det);
Status view_album(detail *mem_det);
Status view_year(detail *mem_det);
Status view_content(detail *mem_det);
Status view_comment(detail *mem_det);
Status display_contents(detail *mem_det);

#endif