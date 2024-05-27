/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/
//
//#ifdef L298_MOTOR_DRIVER
//  #define RIGHT_MOTOR_BACKWARD 5
//  #define LEFT_MOTOR_BACKWARD  6
//  #define RIGHT_MOTOR_FORWARD  9
//  #define LEFT_MOTOR_FORWARD   10
//  #define RIGHT_MOTOR_ENABLE 12
//  #define LEFT_MOTOR_ENABLE 13
//#endif
//
//#ifdef ORIENTAL_MOTOR_DRIVER_ESP
//  #define DIRECTION_MOTOR_RIGHT 14
//  #define PWM__MOTOR_RIGHT      12
//  #define BRAKE_MOTOR_RIGHT     33
//  #define DIRECTION_MOTOR_LEFT  27
//  #define PWM_MOTOR_LEFT        26
//  #define BRAKE_MOTOR_LEFT      25
//#endif

#ifdef ORIENTAL_MOTOR_DRIVER
  #define DIRECTION_MOTOR_RIGHT 10
  #define PWM_MOTOR_RIGHT       9
  #define BRAKE_MOTOR_RIGHT     2
  #define DIRECTION_MOTOR_LEFT  7
  #define PWM_MOTOR_LEFT        6
  #define BRAKE_MOTOR_LEFT      3
#endif

#ifdef ORIENTAL_MOTOR_DRIVER_PC
  #define DIRECTION_MOTOR_RIGHT 10
  #define PWM_MOTOR_RIGHT       9
  #define BRAKE_MOTOR_RIGHT     2
  #define DIRECTION_MOTOR_LEFT  7
  #define PWM_MOTOR_LEFT        6
  #define BRAKE_MOTOR_LEFT      3
#endif


void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
