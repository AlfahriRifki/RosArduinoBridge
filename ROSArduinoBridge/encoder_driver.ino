/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ORIENTAL_ENC_COUNTER_PC
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;

  // /* Interrupt routine for LEFT encoder, taking care of actual counting */
  // ISR(PCINT2_vect) {
  //     left_enc_pos += 1; // Increment position for left encoder
  // }

  // /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  // ISR(PCINT0_vect) {
  //     right_enc_pos += 1; // Increment position for right encoder
  // }

  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR(PCINT2_vect) {
      if (digitalRead(DIRECTION_MOTOR_LEFT) == HIGH) {
          left_enc_pos += 1; // Increment position for left encoder
      } else {
          left_enc_pos -= 1; // Decrement position for left encoder
      }
  }

  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR(PCINT0_vect) {
      if (digitalRead(DIRECTION_MOTOR_RIGHT) == LOW) {
          right_enc_pos += 1; // Increment position for right encoder
      } else {
          right_enc_pos -= 1; // Decrement position for right encoder
      }
  }

  /* Wrap the encoder reading function */
  long readEncoder(int i) {
      if (i == LEFT) return left_enc_pos;
      else return right_enc_pos;
  }

  /* Reset the encoder position */
  void resetEncoder(int i) {
      if (i == LEFT) {
          left_enc_pos = 0L; // Reset left encoder position
      } else {
          right_enc_pos = 0L; // Reset right encoder position
      }
  }

 #elif defined(ORIENTAL_ENC_COUNTER)
  volatile long left_enc_pos = 0;
  volatile long right_enc_pos = 0;

  // Increment the number of pulses by 1
  void left_wheel_pulse() {
    left_enc_pos++;
  }
  void right_wheel_pulse() {
    right_enc_pos++;
  }

  /* Wrap the encoder reading function */
  long readEncoder(int i) {
      if (i == LEFT) return left_enc_pos;
      else return right_enc_pos;
  }

  /* Reset the encoder position */
  void resetEncoder(int i) {
      if (i == LEFT) {
          left_enc_pos = 0; // Reset left encoder position
      } else {
          right_enc_pos = 0; // Reset right encoder position
      }
  }

#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif
