/******************************************************************************
                         Assignment 4: Bitpack.c             
                    COMP 40 Fall 2014: Professor Noah Mendelson              
                     By: Susannah Church and Margaret Feltz                  
                              October 14, 2014                             
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "bitpack.h"
#include "except.h"
#include "assert.h"

const unsigned FULL_SHIFT = 64;
const uint64_t  NUM_BITS = sizeof(uint64_t) * 8;

static inline uint64_t shift_left(uint64_t value, unsigned shift);
static inline uint64_t shift_right(uint64_t value, unsigned shift);
static inline int64_t shift_rights(int64_t value, unsigned shift);

/* Function returns true if n can be represented in width unsigned bits,
and returns false if not */
bool Bitpack_fitsu(uint64_t n, unsigned width) {
        return(shift_left(1, width) - 1 >= n);
}

/* Function returns true if n can be represented in width signed bits, 
   and returns false if not */
bool Bitpack_fitss(int64_t n, unsigned width) {
        if ((n <= ((int64_t)(shift_left(1, width-1) - 1))) 
            && (n >= (int64_t)(-(shift_left(1, width - 1))))) {
                return true;
        }
        return false;
}

/* Function extracts and returns an unsigned value from a word using the masking
   and shifting method */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
        assert(width <= 64);
        assert((width + lsb) <= 64);

        if (width == 0) {
                return 0;
        }
        
        uint64_t mask = ~0;
        mask = shift_left(shift_right(mask, (NUM_BITS - width)), lsb);
        return shift_right((word & mask), lsb);
}

/* Function extracts and returns a signed value from a word using the masking
   and shifting method */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
        if (width == 0) {
                return 0;
        }
        uint64_t got = Bitpack_getu(word, width, lsb);
        uint64_t check = shift_right(got,(width - 1));
        if (check == 1) {
                uint64_t mask = ~0;
                mask = shift_left(mask, width);
                return (int64_t)(got | mask);
        }
        return (int64_t)got;
}

/* Function will return a word with an new unsigned value replacement */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value) {
        assert(width <= 64);
        assert((width + lsb) <= 64);

        if (Bitpack_fitsu(value, width) == false) {
                RAISE(Bitpack_Overflow);
                exit(1);
        }

        uint64_t mask = ~0;
        mask = ~(shift_left(shift_right(mask, (NUM_BITS - width)), lsb));
        uint64_t new_word = ((word & mask) | shift_left(value,lsb));
        return new_word; 
}

/* Function will return a word with an new signed value replacement */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value) {
        assert(width <= 64);
        assert((width + lsb) <= 64);

        if (Bitpack_fitss(value, width) == false) {
                RAISE(Bitpack_Overflow);
                exit(1);
        }

        value = shift_left(value, NUM_BITS - width);
        value = shift_rights(value, NUM_BITS - width);
        uint64_t new_word = Bitpack_newu(word, width, lsb, value);

        return new_word;         
}

/* Exception is raised if value does not fit in width bits, which is signed
   or unsigned depending on the function using the execption */
Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* Shift functions */
/*shift left for unsigned and signed values, ensures 64 bit shifts
are handled sensibly*/
static inline uint64_t shift_left(uint64_t value, unsigned shift) {
        if (shift == FULL_SHIFT) {
                value = (uint64_t)value <<  (FULL_SHIFT - 1);
                value = (uint64_t)value << 1;
        }
        else {
                value = (uint64_t)value << shift;
        }
        return value;
}

/*shift right for unsigned values, ensures 64 bit shifts
are handled sensibly*/
static inline uint64_t shift_right(uint64_t value, unsigned shift) {
        if (shift == FULL_SHIFT) {
                value = (uint64_t)value >>  (FULL_SHIFT - 1);
                value = (uint64_t)value >> 1;
        }
        else {
                value = (uint64_t)value >> shift;
        }
        return value;
}

/*shift right for signed values, ensures 64 bit shifts
are handled sensibly*/
static inline int64_t shift_rights(int64_t value, unsigned shift) {
        if (shift == FULL_SHIFT) {
                value = (uint64_t)value >>  (FULL_SHIFT - 1);
                value = (uint64_t)value >> 1;
        }
        else {
                value = (uint64_t)value >> shift;
        }
        return value;
}
