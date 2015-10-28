#include "instructions.h"

const unsigned NUM_REG = 8;
const unsigned MAX_INPUT_VALUE = 256;

/*-----------------------------------------------------------------------------
Called in loadp to copy the segment with ID == segment_ID into segment 0
-----------------------------------------------------------------------------*/
static void copy_segment_to_m0(UM_machine machine, UM_segment_ID segment_ID);


extern void movc(void *machine_p, unsigned A, unsigned B, unsigned C)
{       
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rC = UArray_at(registers, C);

        /* if $r[C] != 0, $r[A] = $r[B] */
        if(*rC != 0){
                UM_word *rA = UArray_at(registers, A);
                UM_word *rB = UArray_at(registers, B);

                *rA = *rB;
        }
}

extern void segl(void *machine_p, unsigned A, unsigned segmentID_reg,
                 unsigned offset_reg) 
{
        UM_machine machine = (UM_machine) machine_p;
        UM_address_space address_space = machine->address_space;
        UArray_T registers = machine->registers;

        Seq_T mapped_segments = address_space->mapped_segments;

        UM_word segmentID = *((UM_word *) UArray_at(registers, segmentID_reg));
        UM_word offset = *((UM_word *) UArray_at(registers, offset_reg));
        UM_segment segment = Seq_get(mapped_segments, segmentID);

        UM_word *word_p = UArray_at(segment->words, offset);
        UM_word *rA = UArray_at(registers, A);

        /* $r[A] = $m[$r[B]][$r[C]] */
        *rA = *word_p;
}


extern void segs(void *machine_p, unsigned segmentID_reg, unsigned offset_reg,
                 unsigned C)
{       
        UM_machine machine = (UM_machine) machine_p;
        UM_address_space address_space = machine->address_space;
        UArray_T registers = machine->registers;

        Seq_T mapped_segments = address_space->mapped_segments;

        UM_word segmentID = *((UM_word *) UArray_at(registers, segmentID_reg));
        UM_word offset = *((UM_word *) UArray_at(registers, offset_reg));
        UM_segment segment = Seq_get(mapped_segments, segmentID);

        UM_word *word_p = UArray_at(segment->words, offset);
        UM_word *rC = UArray_at(registers, C);

        /*$m[$r[A]][$r[B]] = $r[C]*/
        *word_p = *rC;
}

extern void add(void *machine_p, unsigned A, unsigned B, unsigned C)
{                      
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rB = UArray_at(registers, B);
        UM_word *rC = UArray_at(registers, C);

        UM_word sum = *rB + *rC;
        
        UM_word *rA = UArray_at(registers, A);
        *rA = sum;
}

extern void mult(void *machine_p, unsigned A, unsigned B, unsigned C)
{      
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rB = UArray_at(registers, B);
        UM_word *rC = UArray_at(registers, C);

        UM_word product = *rB * *rC;

        UM_word *rA = UArray_at(registers, A);
        *rA = product;
}

extern void divi(void *machine_p, unsigned A, unsigned B, unsigned C)
{       
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rB = UArray_at(registers, B);
        UM_word *rC = UArray_at(registers, C);

        UM_word quotient = *rB / *rC;
        
        UM_word *rA = UArray_at(registers, A);
        *rA = quotient;
}

extern void nand(void *machine_p, unsigned A, unsigned B, unsigned C)
{
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rB = UArray_at(registers, B);
        UM_word *rC = UArray_at(registers, C);

        UM_word nand = ~(*rB & *rC);

        UM_word *rA = UArray_at(registers, A);
        *rA = nand;
}

extern void map(void *machine_p, unsigned B, unsigned C)
{
        map_segment(machine_p, B, C);
}

extern void unmap(void *machine_p, unsigned C)
{
        unmap_segment(machine_p, C);
}

extern void out(void *machine_p, unsigned C)
{       
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rC = UArray_at(registers, C);
        UM_word output = *rC;
        
        putc(output, stdout);
}

extern void in(void *machine_p, unsigned C)
{
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word store_value = 0;
        int input = getchar();

        if(input < 0){ 
                store_value = ~store_value; 
        }
        else{
                store_value = input;
                assert(store_value < 256);
        }

        UM_word *rC = UArray_at(registers, C);
        *rC = store_value;
}

extern void loadp(void *machine_p, unsigned B,
                  unsigned C)
{
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rB = UArray_at(registers, B);

        /* no need to duplicate if *rB = 0 */
        if(*rB != 0){
                copy_segment_to_m0(machine, *rB);
        }

        UM_word *rC = UArray_at(registers, C);
        machine->program_counter = *rC;
}

extern void loadv(void *machine_p, UM_word value, unsigned A)
{       
        UM_machine machine = (UM_machine) machine_p;
        UArray_T registers = machine->registers;

        UM_word *rA = (UM_word *) UArray_at(registers, A);

        *rA = value;
}

static void copy_segment_to_m0(UM_machine machine, UM_segment_ID segment_ID)
{
        Seq_T mapped_segments = machine->address_space->mapped_segments;
        UM_segment temp = Seq_get(mapped_segments, segment_ID);
        int temp_length = UArray_length(temp->words);

        UM_segment duplicate = get_new_segment_of_size(temp_length);

        /* copy words into the new segment */
        for(int i=0; i<temp_length; i++){
                UM_word *word_i_p = UArray_at(temp->words, i);
                UM_word *duplicate_word_i_p = UArray_at(duplicate->words, i);
                *duplicate_word_i_p = *word_i_p;
        }

        duplicate->ID = 0;

        /* free the old program segment */
        UM_segment program = Seq_get(mapped_segments, 0);
        free_segment_memory(&program);

        /* put the duplicate segment in the place of the old program segment */
        Seq_put(mapped_segments, 0, duplicate);

        /* update num_instructions */
        machine->num_instructions = temp_length;
}