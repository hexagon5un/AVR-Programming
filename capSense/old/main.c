/*  */

// ------- Preamble -------- //
#include "main.h"

int main(void){
  
  // -------- Inits --------- //
  uint8_t count;

  initUSART();
  transmitString("Cap Sense Demo\r\n");
  initTimer0();
  
  LED_DDR = 0xff;		/* all output */
  set_bit(LED_PORT, LED3);
  _delay_ms(100);
  clear_bit(LED_PORT, LED3);

  // ------ Event loop ------ //
  while(1){			
    
    count = pollSensor();
    printByte(count);
    transmitString("\r\n");

    if (count > THRESHOLD){
      set_bit(LED_PORT, LED3);
    }
    else{
      clear_bit(LED_PORT, LED3);
    }
    
    _delay_ms(100);
    

  }    /* End event loop */
  return(0);		      /* This line is never reached  */
}

static inline void initTimer0(void){
  /* No WGM bits need to be set for normal / counter mode */
  /* No output set either */
  set_bit(TCCR0B, CS00);	/* running full speed */
}

static inline uint8_t pollSensor(void){
  set_bit(CAP_SENSOR_DDR, CAP_SENSOR); /* output mode */
  clear_bit(CAP_SENSOR_PORT, CAP_SENSOR); /* set low to discharge capacitor */
  _delay_ms(1);			  /* a short while */
    
  clear_bit(CAP_SENSOR_DDR, CAP_SENSOR); /* set as input */
  set_bit(CAP_SENSOR_PORT, CAP_SENSOR);  /* set pullup to charge capacitor */
  TCNT0 = 0;				   /* zero counter */
  loop_until_bit_is_set(CAP_SENSOR_IN, CAP_SENSOR);
  return(TCNT0);
}
