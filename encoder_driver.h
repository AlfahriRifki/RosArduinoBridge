/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifdef ORIENTAL_ENC_COUNTER_PC
  //below can be changed, but should be PORTD pins; 
  //otherwise additional changes in the code are required
  #define LEFT_ENC_PIN PK1  //pin A9 - PCINT[17]
  #define RIGHT_ENC_PIN PB5  //pin D11 - PCINT[5]
  
#elif defined ORIENTAL_ENC_COUNTER
  #define LEFT_ENC_PIN A1  
  #define RIGHT_ENC_PIN A0  
  
#endif
   
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
