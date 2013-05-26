
/* Standard Macros */
/* You can totally get by without these, but why? */

/* Make sure we've already got io / sfr / pindefs loaded */
#ifndef   _AVR_IO_H_
#include  <avr/io.h>
#endif

/* Reminder: the following useful bit-twiddling macros are
   always included in avr/sfr_defs.h, which is called from
   avr/io.h 

 bit_is_set(sfr, bit)
 bit_is_clear(sfr, bit)
 loop_until_bit_is_set(sfr, bit)
 loop_until_bit_is_clear(sfr, bit)

*/

/* Define up the full complement of bit-twiddling macros */
#define BV(bit)               (1 << bit)
#define set_bit(sfr, bit)     (_SFR_BYTE(sfr) |= BV(bit))  // old sbi()
#define clear_bit(sfr, bit)   (_SFR_BYTE(sfr) &= ~BV(bit)) // old cbi()
#define toggle_bit(sfr, bit)  (_SFR_BYTE(sfr) ^= BV(bit))  

