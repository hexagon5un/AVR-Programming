
/* A bunch of defines that I'll use from time to time */

#ifndef __COMMON.h 
#define __COMMON.h 
#endif

#ifndef _SFR_BYTE 
#include  <avr/io.h>
#endif


/* Define up the full complement of bit-twiddling macros */
#define BV(bit)               (1 << bit)
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit))
#define toggle_bit(sfr, bit)  (_SFR_BYTE(sfr) ^= BV(bit))

#ifndef bit_is_set
#define bit_is_set(sfr, bit)                (_SFR_BYTE(sfr) & BV(bit))
#define bit_is_clear(sfr, bit)              (!(_SFR_BYTE(sfr) & BV(bit)))
#define loop_until_bit_is_set(sfr, bit)     do { } while (bit_is_clear(sfr, bit))
#define loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))
#endif

