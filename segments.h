/*
 *      segments.h
 *      Authors: Brett Gurman and Margaret Feltz
 *      Created on Tuesday, November 4 2014
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <uarray.h>
#include <seq.h>
#include <assert.h>
#include <bitpack.h>
 

typedef uint32_t UM_segment_ID;
typedef uint32_t UM_word;

typedef struct UM_address_space {
        Seq_T mapped_segments;          /* contains all currently mapped
                                           segments */
        Seq_T reusable_IDs;             /* contains all segment IDs that have
                                           been unmapped */
        UM_segment_ID ID_counter;       /* keeps track of the first never-used
                                           segment ID */
} *UM_address_space;

typedef struct UM_machine {
        UM_address_space address_space;
        UArray_T registers;
        uint32_t program_counter;
        unsigned num_instructions;
} *UM_machine;

typedef struct UM_segment {
        UArray_T words;            /* the words in the segment */
        UM_segment_ID ID;       /* the ID of the segment */
} *UM_segment;
/*-----------------------------------------------------------------------------
Given a pointer to the machine, and unsigned ints B and C, this function maps a
segment with length == $r[C], bitpacks the ID of the mapped segment, and stores
the bitpacked ID in $r[B].

void *machine_p: a pointer to the machine struct.
unsigned B: the bitpacked ID is placed into the register specified by this 
            value.
unsigned C: a segment is mapped with length equal to the contents of the
            register specified by this value.
-----------------------------------------------------------------------------*/
extern void map_segment(void *machine_p, unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine, this function unmaps the segment $m[$r[C]],
which may be used by future map instructions.

void *machine_p: a pointer to the machine.
unsigned C:      the number for the register whose value is equal to the 
                 segment ID of segment to be unmapped. 
-----------------------------------------------------------------------------*/
extern void unmap_segment(void *machine_p, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to a segment, this function frees the memory allocated for the
segment.

UM_segment *segment: the name of the segment to be mapped.
-----------------------------------------------------------------------------*/
extern void free_segment_memory(UM_segment *segment);

/*-----------------------------------------------------------------------------
Given a size, this function creates a segment of that size.

unsigned size: the size of the segment to be mapped.
-----------------------------------------------------------------------------*/
extern UM_segment get_new_segment_of_size(unsigned size);

