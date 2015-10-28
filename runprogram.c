/******************************************************************************
                                runprogram.c
                  Authors: Brett Gurman and Margaret Feltz
                  Created on Wednesday, November 12th 2014
*************************************************************************/    

#include <runprogram.h>

/*-----------------------------------------------------------------------------
This function is called by void run_program in a loop, executing one
instruction cycle with each call It executes the instruction that resides
within the program segment at an offset equal to the universal machine's
program counter. Return value is 0 if that instruction was halt or if the
program counter has run out of bounds, and 1 otherwise. 
-----------------------------------------------------------------------------*/
static int execute_cycle(UM_machine machine);

/*-----------------------------------------------------------------------------
This function is called when the program has finished running and is
responsible for freeing all memory associated with the universal machine, 
including all components of the address space and the address space itself.
-----------------------------------------------------------------------------*/
static void free_machine(UM_machine machine);

/*-----------------------------------------------------------------------------
This is an auxiliary function called by void free_machine(UM_machine machine), 
it frees the memory associated with the address space including all components 
of the address space and the address space itself.  
-----------------------------------------------------------------------------*/
static void free_address_space(UM_machine machine);


extern void run_program(void *machine_p)
{
        assert(machine_p);

        UM_machine machine = (UM_machine) machine_p;

        while(true){
                /* stop running if halt instruction reached or if program 
                   counter has gone out of bounds */
                if(!execute_cycle(machine)){
                        break;
                }
        }

        free_machine(machine);
}

static int execute_cycle(UM_machine machine)
{
        Seq_T mapped_segments = machine->address_space->mapped_segments;
        UM_segment program_segment = Seq_get(mapped_segments, 0);
        unsigned num_instructions = machine->num_instructions;

        /* stop if program counter has run out of bounds */
        if(machine->program_counter >= num_instructions) return 0;


        /* retreive the next instruction */
        Um_instruction new_instruction = 
                      *((Um_instruction *)UArray_at(program_segment->words,
                                                    machine->program_counter));

        /* increment the program counter */
        machine->program_counter++;

        /* decode and execute the instruction retreived */
        return decode_instruction(machine, &new_instruction);
}

static void free_machine(UM_machine machine)
{
        free_address_space(machine);
        UArray_free(&(machine->registers));
        free(machine);
}

static void free_address_space(UM_machine machine)
{       
        UM_address_space address_space = machine->address_space;

        /* free the mapped segments sequence */
        int length = Seq_length(address_space->mapped_segments);
        for(int i=0; i<length; i++){
                UM_segment segment = Seq_remlo(address_space->mapped_segments);

                /* if segment with ID == i is mapped, free it */
                if(segment != NULL){
                        free_segment_memory(&segment);
                }
        }
        Seq_free(&address_space->mapped_segments);

        /* free the reusable IDs sequence */
        for(int i=0; i<Seq_length(address_space->reusable_IDs); i++){
                UM_segment_ID *seg_ID = 
                                       Seq_get(address_space->reusable_IDs, i);
                free(seg_ID);
        }
        Seq_free(&(address_space->reusable_IDs));

        free(address_space);
}
