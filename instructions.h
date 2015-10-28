#include "segments.h"

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints A, B, and C, this
function places the contents of register B into register A, provided that
the contents of register C are not 0.

void* machine_p: a pointer to the machine.
unsigned A:      the number of the register into which the contents
                 of register B are to be placed.
unsigned B:      the number of the register whose contents are to be
                 moved to register A.
unsigned C:      the number of the register whose contents are to be
                 compared to 0.

return value:    no return value.

A checked runtime error results if machine_p is NULL or if A, B, or C is
greater than the total number of registers.
-----------------------------------------------------------------------------*/
extern void movc(void *machine_p, unsigned A, unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints A, segmentID, and offset, this
function places the word $m[$r[segmentID]][$r[offset]] into register A. 

void *machine_p:    a pointer to the machine
unsigned A:         the number of the register into which the word 
                    $m[$r[B]][$r[C]] is to be moved.
unsigned segmentID: the ID of the segment containing the word to be moved into 
                    register A.
unsigned offset:    the offset within segment $m[segmentID] that specifies the
                    word to be moved into register A. 
-----------------------------------------------------------------------------*/
extern void segl(void *machine_p, unsigned A, unsigned segmentID_reg, 
                 unsigned offset_reg);


/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints segmentID, offset, and C this
function stores the word in register C as $m[$r[segmentID]][$r[offset]].

void *machine_p:    a pointer to the machine
unsigned segmentID: the ID of the segment containing the word to be moved into 
                    register C.
unsigned offset:    the offset within segment $m[$r[segmentID]] that specifies
                    the word to be moved into register C. 
-----------------------------------------------------------------------------*/
extern void segs(void *machine_p, unsigned segmentID_reg, unsigned offset_reg,
                 unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints A, B, and C, this function
places the sum of the values in registers B and C mod 2^32 into register A.

void *machine_p:  a pointer to the machine.
unsigned A:       the number of the register into which the sum will be placed.
unsigned B and C: the numbers of the two registers whose contents are to be 
                  added. 
-----------------------------------------------------------------------------*/
extern void add(void *machine_p, unsigned A, unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints A, B, and C, this function
places the product the values in registers B and C mod 2^32 into register A.

void *machine_p:  a pointer to the machine.
unsigned A:       the number of the register into which the product will be
                  placed.
unsigned B and C: the numbers of the two registers whose contents are to be 
                  multiplied. 
-----------------------------------------------------------------------------*/
extern void mult(void *machine_p, unsigned A, unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints A, B, and C, this function
places the quotient of the values in registers B and C into register A.

void *machine_p:  a pointer to the machine.
unsigned A:       the number of the register into which the quotient will be
                  placed.
unsigned B and C: the numbers of the two registers whose contents are to be 
                  divided. 
-----------------------------------------------------------------------------*/
extern void divi(void *machine_p, unsigned A, unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints A, B, and C, this function
places the result of executing the not-and operation on the values in registers
B and C into register A.

void *machine_p:  a pointer to the machine.
unsigned A:       the number of the register into which the result will be
                  placed.
unsigned B and C: the numbers of the two registers whose contents are to be 
                  nand'd. 
-----------------------------------------------------------------------------*/
extern void nand(void *machine_p, unsigned A, unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints B and C, this function creates
a new segment with length equal to the value in register C, initializes each 
word in the segment to zero, places a bit pattern that is not all zeroes and
does not identify a currently mapped segment into register B, and maps the new
segment as $m[$r[B]].

void *machine_p: a pointer to the machine.
unsigned B:      the number for the register into which will be placed the bit 
                 pattern representing the ID of the new segment.
unsigned C:      the number for the register whose value equals the length, in 
                 number of words, of the segment to be mapped.
-----------------------------------------------------------------------------*/
extern void map(void *machine_p, unsigned B, unsigned C);
 
/*-----------------------------------------------------------------------------
Given a pointer to the machine, this function unmaps the segment $m[$r[C]],
which may be used by future map instructions.

void *machine_p: a pointer to the machine.
unsigned C:      the number for the register whose value is equal to the 
                 segment ID of segment to be unmapped. 
-----------------------------------------------------------------------------*/
extern void unmap(void *machine_p, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned int C, this function immediately 
displays to the I/O device the contents of register C. A given word is valid
output if and only if its value is between 0 and 255.

void *machine_p: a pointer to the machine.
unsigned C:      the number of the register whose contents are to be outputted.
-----------------------------------------------------------------------------*/
extern void out(void *machine_p, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned int C, this function waits for 
input and places that input in register C. Input is valid if and only if it is 
in the form of a value from 0 to 255. If the end of input has been signaled,
register C is loaded with a full 32-bit word consisting of all 1s. 

void *machine_p: a pointer to the machine.
unsigned C:      the number of the register into which input is to be placed.
-----------------------------------------------------------------------------*/
extern void in(void *machine_p, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine and unsigned ints B and C, this function makes a
duplicate of segment $m[$r[B]], which replaces $m[0], and sets the program 
counter to point $m[0][$r[C]]. 

void *machine_p: a pointer to the machine.
unsigned B:      the number of the register whose contents are the ID of the
                 segment to replace the program segment.
unsigned C:      the number of the register whose contents are the value that
                 the program counter is to be set to.
-----------------------------------------------------------------------------*/
extern void loadp(void *machine_p, unsigned B, unsigned C);

/*-----------------------------------------------------------------------------
Given a pointer to the machine, UM_word value, and unsigned int A, this
function places value into register A.

void *machine_p: a pointer to the machine
UM_word value:   the value to be placed into register A.
unsigned A:      the number of the register into which the given value is to be
                 placed.
-----------------------------------------------------------------------------*/
extern void loadv(void *machine_p, UM_word value, unsigned A);

