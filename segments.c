#include "segments.h"

/*-----------------------------------------------------------------------------
Called by map_segment to map a segment when there are reusable IDs available.
-----------------------------------------------------------------------------*/
static void map_old_ID(UArray_T registers, UM_address_space address_space, 
                       unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Called by map_segment to map a segment when there aren't any reusable IDs
-----------------------------------------------------------------------------*/
static void map_new_ID(UArray_T registers, UM_address_space address_space,
                       unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Called when an ID is mapped to bitpack the segment ID and place it in the 
appropriate register.
-----------------------------------------------------------------------------*/
static void store_ID_in_register(UArray_T registers, unsigned I,
                                   UM_segment_ID segment_ID);

extern void map_segment(void *machine_p, unsigned B, unsigned C)
{   
        assert(B < 8 && C < 8);
        
        UM_machine machine = (UM_machine) machine_p;
        UM_address_space address_space = machine->address_space;
        UArray_T registers = machine->registers;

        /* if there is a reusable ID, reuse it */
        if(Seq_length(address_space->reusable_IDs) > 0){
                map_old_ID(registers, address_space, B, C);
        }
        /* otherwise, use a new ID */
        else{
                map_new_ID(registers, address_space, B, C);
        }
}

extern void unmap_segment(void *machine_p, unsigned C)
{
        assert(C < 8);

        UM_machine machine = (UM_machine) machine_p;
        UM_address_space address_space = machine->address_space;
        UArray_T registers = machine->registers;

        Seq_T mapped_segments = address_space->mapped_segments;

        UM_segment_ID *segment_ID = malloc(sizeof(UM_segment_ID));
        assert(segment_ID);

        UM_word *rC = UArray_at(registers, C);
        *segment_ID = *rC;

        UM_segment segment = (UM_segment)Seq_get(mapped_segments, *segment_ID);

        /* "remove" the old segment from the mapped segments structure and
           free the used memory */
        Seq_put(mapped_segments, *segment_ID, NULL); /*****/
        free_segment_memory(&segment);

        /* add the ID of the unmapped segment to the list of reusable IDs */
        Seq_addhi(address_space->reusable_IDs, segment_ID);
}


static void map_old_ID(UArray_T registers, UM_address_space address_space, 
                       unsigned B, unsigned C)
{       
        UM_word *rC = UArray_at(registers, C);
        UM_segment new_segment = get_new_segment_of_size(*rC);

        /* set new_segment->ID to the first reusable segment ID, then free that 
           ID */
        UM_segment_ID *new_ID = Seq_remlo(address_space->reusable_IDs);
        new_segment->ID = *new_ID;
        free(new_ID);

        /* bitpack ID and store in register B */
        store_ID_in_register(registers, B, new_segment->ID);

        /* put the segment into the mapped segments structure */
        Seq_put(address_space->mapped_segments, new_segment->ID, new_segment);
}

static void map_new_ID(UArray_T registers, UM_address_space address_space,
                       unsigned B, unsigned C)
{       
        UM_word *rC = UArray_at(registers, C);

        UM_segment new_segment = get_new_segment_of_size(*rC);
        new_segment->ID = address_space->ID_counter; /* grab the ID */
        address_space->ID_counter++; /* increment the ID counter */

        /* bitpack ID and store in register B */
        store_ID_in_register(registers, B, new_segment->ID);

        /* put the segment into the mapped segments structure */
        Seq_addhi(address_space->mapped_segments, new_segment);
}

extern UM_segment get_new_segment_of_size(unsigned size)
{
        UM_segment new_segment = malloc(sizeof(struct UM_segment));
        assert(new_segment);

        new_segment->words = UArray_new(size, sizeof(UM_word));

        /* initialize words to 0 */
        for(unsigned i=0; i<size; i++){ 
                UM_word new_word = 0;
                UM_word *word_i_p = UArray_at(new_segment->words, i);

                *word_i_p = new_word;
        }

        return new_segment;
}

extern void free_segment_memory(UM_segment *segment)
{       
        /* free the segment's words */
        UArray_T words = (*segment)->words;
        UArray_free(&words);

        /* free the segment itself */
        free(*segment);
}

static void store_ID_in_register(UArray_T registers, unsigned I,
                                   UM_segment_ID segment_ID)
{
        UM_word bitpacked_ID = Bitpack_newu(0, 32, 0, segment_ID);

        UM_word *rI = UArray_at(registers, I);
        *rI = bitpacked_ID;
}