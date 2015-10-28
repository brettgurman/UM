/******************************************************************************
                                runprogram.h
                  Authors: Brett Gurman and Margaret Feltz
                  Created on Wednesday, November 12th 2014
******************************************************************************
*/  


#include <decodeinstr.h>

/*-----------------------------------------------------------------------------
Given a pointer to a machine, this function runs the program segment, i.e the
segment of ID 0.

void *machine_p: The machine on which a program is to be run.

A checked runtime error will result if machine_p is NULL.
-----------------------------------------------------------------------------*/
extern void run_program(void *machine_p);
