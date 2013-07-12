
// Include file for half-stepping order
// You may need to change the software pinouts around to match your motor
//   or change your motor connections around to match the software.

const uint8_t stepOrder[] = {
  (1<<PB0) | (1<<PB2),
  (1<<PB0),
  (1<<PB0) | (1<<PB3),
  (1<<PB3),
  (1<<PB1) | (1<<PB3),
  (1<<PB1),
  (1<<PB1) | (1<<PB2),
  (1<<PB2)
};

#define LAST_PHASE_IN_CYCLE  7


