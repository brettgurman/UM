/******************************************************************************
                                    um.c
                  Authors: Brett Gurman and Margaret Feltz
                  Created on Wednesday, November 12th 2014
*****************************************************************************/ 

/*-----------------------------------------------------------------------------
-----------------------------------------------------------------------------*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "runprogram.h"


const unsigned BYTES_PER_INSTRUCTION = 4;
const unsigned BITS_PER_CHAR = 8;


typedef uint32_t UM_instruction;


/*-----------------------------------------------------------------------------
This function initializes a machine with the program segment loaded with ID 0.
-----------------------------------------------------------------------------*/
static UM_machine get_new_machine_with_program(char *file);

/*-----------------------------------------------------------------------------
Allocates memory for a UM_machine.
-----------------------------------------------------------------------------*/
static UM_machine get_new_machine();

/*-----------------------------------------------------------------------------
Allocates memory for a UM_address_space.
-----------------------------------------------------------------------------*/
static UM_address_space get_new_address_space();

/*-----------------------------------------------------------------------------
Allocates memory for a UArray_T of proper size to represent the registers.
-----------------------------------------------------------------------------*/
static UArray_T get_new_registers();

/*-----------------------------------------------------------------------------
Processes the instructions from the input file into a segment of instructions,
i.e. the program segment, and adds that segment to the machine with ID 0.
-----------------------------------------------------------------------------*/
static void add_program_segment(UM_machine machine, char *file);

/*-----------------------------------------------------------------------------
This auxiliary function is called by add_program_segment to grab the next
instruction from the file.
-----------------------------------------------------------------------------*/
static UM_instruction get_next_instruction(FILE *fp);



int main(int argc, char *argv[])
{
        (void) argc;

        /* read intructions from argv[1], i.e. the input file */
        UM_machine machine = get_new_machine_with_program(argv[1]);
        run_program(machine);
}

static UM_machine get_new_machine_with_program(char *file)
{
        UM_machine machine = get_new_machine();
        add_program_segment(machine, file);
        return machine;
}

static UM_machine get_new_machine()
{
        UM_machine machine = malloc(sizeof(struct UM_machine));
        assert(machine);

        machine->address_space = get_new_address_space();
        machine->registers = get_new_registers();
        machine->program_counter = 0;
        machine->num_instructions = 0;

        return machine;
}

static UM_address_space get_new_address_space()
{
        UM_address_space address_space = 
                                       malloc(sizeof(struct UM_address_space));
        assert(address_space);

        address_space->mapped_segments = Seq_new(50);
        address_space->reusable_IDs = Seq_new(10);
        address_space->ID_counter = 0;

        return address_space;
}

static UArray_T get_new_registers()
{
        UArray_T registers = UArray_new(8, sizeof(UM_word));
        
        /* initialize each register to 0 */
        for(int i=0; i<8; i++){
                UM_word *rI = UArray_at(registers, i);
                *rI = 0;
        }

        return registers;
}

static void add_program_segment(UM_machine machine, char *file)
{
        struct stat fileStats;
        FILE *FILE = fopen(file, "r");

        /* exit if the file does not exist or if stat fails */
        if(stat(file, &fileStats) < 0){
                fprintf(stderr, "Stat failed. Invalid file.\n");
                exit(EXIT_FAILURE);
        }

        int fileSize = fileStats.st_size;

        /* exit if fileSize indicates the wrong number of bytes */
        if(fileSize%4 != 0){
                fprintf(stderr, "Program file formatted incorrectly.\n");
                exit(EXIT_FAILURE);
        }

        int num_instructions = fileSize / BYTES_PER_INSTRUCTION;

        UM_segment program_segment = get_new_segment_of_size(num_instructions);

        /* get the instructions from the input file */
        for(int i=0; i<num_instructions; i++){
                UM_instruction next_instruction = get_next_instruction(FILE);
                UM_instruction *instruction_i_p = 
                                          UArray_at(program_segment->words, i);
                                          
                *instruction_i_p = next_instruction;
        }

        /* put the program segment in the mapped segments sequence */
        Seq_addlo(machine->address_space->mapped_segments, program_segment);
        machine->address_space->ID_counter++;

        machine->num_instructions = num_instructions;

        fclose(FILE);
}

static UM_instruction get_next_instruction(FILE *fp)
{ 
        int c;
        UM_instruction next_instruction = 0;

        /* bitpack the instruction, 1 byte at a time */
        for(unsigned i=0; i < BYTES_PER_INSTRUCTION; i++){
                c = getc(fp);
                next_instruction = 
                Bitpack_newu(next_instruction, BITS_PER_CHAR,
                             24-(BITS_PER_CHAR * i), c);
        }

        return next_instruction;
} 
