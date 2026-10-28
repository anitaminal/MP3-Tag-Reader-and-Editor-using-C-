#ifndef EDIT_PROCESS_H
#define EDIT_PROCESS_H

#include "data.h"   
#include "process.h"


Status check_function(char *argv[], detail *mem_det);
Status validate_edit_args(char *argv[], detail *mem_det);
Status open_edit_files(char *argv[], detail *mem_det);
Status edit_contents(detail *mem_det);
Status edit_file(char *argv[], detail *mem_det );

#endif