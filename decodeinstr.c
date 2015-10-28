/******************************************************************************
                                decodeinstr.c
                  Authors: Brett Gurman and Margaret Feltz
                  Created on Wednesday, November 12th 2014
******************************************************************************/

#include <decodeinstr.h>

const unsigned NUM_INSTRUCTIONS = 14;
const unsigned BITS_PER_OP_CODE = 4;
const unsigned BITS_PER_REG_NUM = 3;
const unsigned BITS_PER_VALUE = 25;

/*-----------------------------------------------------------------------------
Called for all three register executes.
-----------------------------------------------------------------------------*/
static int three_register_execute(UM_machine machine,
                                  Um_instruction instruction);
/*-----------------------------------------------------------------------------
Called for all load value executes.
-----------------------------------------------------------------------------*/
static void load_value_execute(UM_machine machine, Um_instruction instruction);

extern int decode_instruction(void *machine_p, void *instruction_p)
{
        assert(machine_p && instruction_p);

        UM_machine machine = (UM_machine) machine_p;
        Um_instruction instruction = *((Um_instruction *) instruction_p);

        unsigned op = Bitpack_getu(instruction, BITS_PER_OP_CODE, 28);

        switch(op){
                case 13: 
                        load_value_execute(machine, instruction);
                        return 1;
                default:
                        return three_register_execute(machine, instruction);
        }
}

static int three_register_execute(UM_machine machine,
                                  Um_instruction instruction)
{
        unsigned op = Bitpack_getu(instruction, BITS_PER_OP_CODE, 28);
        unsigned A = Bitpack_getu(instruction, BITS_PER_REG_NUM, 6);
        unsigned B = Bitpack_getu(instruction, BITS_PER_REG_NUM, 3);
        unsigned C = Bitpack_getu(instruction, BITS_PER_REG_NUM, 0);

        switch(op){
                case 0: //
                        movc(machine, A, B, C);
                        return 1;

                case 1:
                        segl(machine, A, B, C);
                        return 1;
                        
                case 2:
                        segs(machine, A, B, C);
                        return 1;

                case 3:
                        add(machine, A, B, C); 
                        return 1;

                case 4:
                        mult(machine, A, B, C);
                        return 1;

                case 5:
                        divi(machine, A, B, C);
                        return 1;

                case 6:
                        nand(machine, A, B, C);
                        return 1;

                case 7:
                        return 0;

                case 8:
                        map(machine, B, C);
                        return 1;

                case 9: 
                        unmap(machine, C);
                        return 1;

                case 10:
                        out(machine, C);
                        return 1;

                case 11:
                        in(machine, C);
                        return 1;

                case 12:
                        loadp(machine, B, C);
                        return 1;
                default:
                        return 0;
        }
}

static void load_value_execute(UM_machine machine, Um_instruction instruction)
{

        unsigned A = Bitpack_getu(instruction, BITS_PER_REG_NUM, 25);
        UM_word value = Bitpack_getu(instruction, BITS_PER_VALUE, 0);

        loadv(machine, value, A);
}