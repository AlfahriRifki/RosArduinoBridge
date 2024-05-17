/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  //encoder lookup table
    
  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR (PCINT2_vect){
  	static uint8_t enc_last=0;
        
	enc_last <<=2; //shift previous state two places
	enc_last |= (PIND & (3 << 2)) >> 2; //read the current state into lowest 2 bits
  
  	left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR (PCINT1_vect){
        static uint8_t enc_last=0;
          	
	enc_last <<=2; //shift previous state two places
	enc_last |= (PINC & (3 << 4)) >> 4; //read the current state into lowest 2 bits
  
  	right_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }
#elif defined(ORIENTAL_ENC_COUNTER_PC)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;

  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR(PCINT2_vect) {
      left_enc_pos += 1; // Increment position for left encoder
  }

  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR(PCINT0_vect) {
      right_enc_pos += 1; // Increment position for right encoder
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
