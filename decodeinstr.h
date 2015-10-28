/******************************************************************************
                                decodeinstr.h
                  Authors: Brett Gurman and Margaret Feltz
                  Created on Wednesday, November 12th 2014
******************************************************************************/

#include <instructions.h>

typedef uint32_t Um_instruction;
typedef short Um_opcode;

/*-----------------------------------------------------------------------------
Given a pointer to a machine and a pointer to an instruction, this function
decodes and executes the instruction. Returns zero if the instruction is a halt
instruction, and 1 otherwise.

void *machine_p: A pointer to the machine on which a program is being run.
void *instruction_p: A pointer to the instruction to be executed.

return value: 0 if the instruction is a halt instruction, 1 otherwise.

A checked runtime error will 
-----------------------------------------------------------------------------*/
extern int decode_instruction(void *machine_p, void *instruction_p);