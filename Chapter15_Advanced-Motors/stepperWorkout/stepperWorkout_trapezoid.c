void trapezoidMove(int16_t howManySteps) {
  uint8_t delay = MAX_DELAY;
  uint16_t stepsTaken = 0;

                               /* set direction, make howManySteps > 0 */
  if (howManySteps > 0) {
    direction = FORWARD;
  }
  else {
    direction = BACKWARD;
    howManySteps = -howManySteps;
  }

  if (howManySteps > (RAMP_STEPS * 2)) {
                             /* Have enough steps for a full trapezoid */
                                                         /* Accelerate */
    while (stepsTaken < RAMP_STEPS) {
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }
                                                             /* Cruise */
    delay = MIN_DELAY;
    takeSteps((howManySteps - 2 * RAMP_STEPS), delay);
    stepsTaken += (howManySteps - 2 * RAMP_STEPS);
                                                         /* Decelerate */
    while (stepsTaken < howManySteps) {
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }
  }
  else {
                                               /* Partial ramp up/down */
    while (stepsTaken <= howManySteps / 2) {
      takeSteps(1, delay);
      delay -= ACCELERATION;
      stepsTaken++;
    }
    delay += ACCELERATION;
    while (stepsTaken < howManySteps) {
      takeSteps(1, delay);
      delay += ACCELERATION;
      stepsTaken++;
    }
  }
}
