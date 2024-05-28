#define USE_BASE      // Enable the base controller code
// #undef USE_BASE     // Disable the base controller code

/* Define the motor controller and encoder library used*/
#ifdef USE_BASE
   /* Encoders on ORIENTAL MOTOR DRIVERS with Pin Change Register*/
   #define ORIENTAL_ENC_COUNTER_PC
   /* Encoders on ORIENTAL MOTOR DRIVERS */
//   #define ORIENTAL_ENC_COUNTER

//   #define ORIENTAL_MOTOR_DRIVER
   #define ORIENTAL_MOTOR_DRIVER_PC
#endif

/* Serial port baud rate */
#define BAUDRATE     57600

/* Maximum PWM signal */
#define MAX_PWM        100

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/* Include definition of serial commands */
#include "commands.h"


#ifdef USE_BASE
  /* Motor driver function definitions */
  #include "motor_driver.h"

  /* Encoder driver function definitions */
  #include "encoder_driver.h"

  /* PID parameters and functions */
  #include "diff_controller.h"

  /* Run the PID loop at 30 times per second */
  #define PID_RATE           30     // Hz

  /* Convert the rate into an interval */
  const int PID_INTERVAL = 1000 / PID_RATE;
  
  /* Track the next time we make a PID calculation */
  unsigned long nextPID = PID_INTERVAL;

  /* Stop the robot if it hasn't received a movement command
   in this number of milliseconds */
  #define AUTO_STOP_INTERVAL 2000
  long lastMotorCommand = AUTO_STOP_INTERVAL;
#endif

/* Variable initialization */

// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];

// The arguments converted to integers
long arg1;
long arg2;

/* Clear the current command parameters */
void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}

/* Run a command.  Commands are defined in commands.h */
int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  
  switch(cmd) {
  case GET_BAUDRATE:
    Serial.println(BAUDRATE);
    break;
  case ANALOG_READ:
    Serial.println(analogRead(arg1));
    break;
  case DIGITAL_READ:
    Serial.println(digitalRead(arg1));
    break;
  case ANALOG_WRITE:
    analogWrite(arg1, arg2);
    Serial.println("OK"); 
    break;
  case DIGITAL_WRITE:
    if (arg2 == 0) digitalWrite(arg1, LOW);
    else if (arg2 == 1) digitalWrite(arg1, HIGH);
    Serial.println("OK"); 
    break;
  case PIN_MODE:
    if (arg2 == 0) pinMode(arg1, INPUT);
    else if (arg2 == 1) pinMode(arg1, OUTPUT);
    Serial.println("OK");
    break;
  // case PING:
  //   Serial.println(Ping(arg1));
  //   break;
    
#ifdef USE_BASE
  case READ_ENCODERS:
    Serial.print(readEncoder(LEFT));
    Serial.print(" ");
    Serial.println(readEncoder(RIGHT));
    break;
   case RESET_ENCODERS:
    resetEncoders();
    resetPID();
    Serial.println("OK");
    break;
  case MOTOR_SPEEDS:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    if (arg1 == 0 && arg2 == 0) {
      setMotorSpeeds(0, 0);
      resetPID();
      moving = 0;
    }
    else moving = 1;
    leftPID.TargetTicksPerFrame = arg1;
    rightPID.TargetTicksPerFrame = arg2;
    Serial.println("OK"); 
    break;
  case MOTOR_RAW_PWM:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    resetPID();
    moving = 0; // Sneaky way to temporarily disable the PID
    setMotorSpeeds(arg1, arg2);
    Serial.println("OK"); 
    break;
  case UPDATE_PID:
    while ((str = strtok_r(p, ":", &p)) != '\0') {
       pid_args[i] = atoi(str);
       i++;
    }
    Kp = pid_args[0];
    Kd = pid_args[1];
    Ki = pid_args[2];
    Ko = pid_args[3];
    Serial.println("OK");
    break;
#endif
  default:
    Serial.println("Invalid Command");
    break;
  }
}

/* Setup function--runs once at startup. */
void setup() {
  Serial.begin(BAUDRATE);
  pinMode(DIRECTION_MOTOR_RIGHT, OUTPUT);
  pinMode(PWM_MOTOR_RIGHT, OUTPUT);
  pinMode(BRAKE_MOTOR_RIGHT, OUTPUT);
  pinMode(DIRECTION_MOTOR_LEFT, OUTPUT);
  pinMode(PWM_MOTOR_LEFT, OUTPUT);
  pinMode(BRAKE_MOTOR_LEFT, OUTPUT);
  
//  analogWrite(PWM_MOTOR_RIGHT, 0);
//  analogWrite(PWM_MOTOR_LEFT, 0);
//  digitalWrite(BRAKE_MOTOR_RIGHT, LOW);
//  digitalWrite(BRAKE_MOTOR_LEFT, LOW);

// Initialize the motor controller if used */
#ifdef USE_BASE
  #ifdef ORIENTAL_ENC_COUNTER_PC
    //set as inputs
    DDRK &= ~(1<<LEFT_ENC_PIN);
    DDRB &= ~(1<<RIGHT_ENC_PIN);
    
    //enable pull up resistors
    PORTK |= (1<<LEFT_ENC_PIN);
    PORTB |= (1<<RIGHT_ENC_PIN);
    
    // tell pin change mask to listen to left encoder pins
    PCMSK2 |= (1 << LEFT_ENC_PIN);
    // tell pin change mask to listen to right encoder pins
    PCMSK0 |= (1 << RIGHT_ENC_PIN);
    
    // enable PCINT1 and PCINT2 interrupt in the general interrupt mask
    PCICR |= (1 << PCIE0) | (1 << PCIE2);
   #elif defined ORIENTAL_ENC_COUNTER
    // Set pin states of the encoder
    pinMode(LEFT_ENC_PIN, INPUT_PULLUP);
    pinMode(RIGHT_ENC_PIN, INPUT_PULLUP);

    // Every time the pin goes high, this is a pulse
    attachInterrupt(digitalPinToInterrupt(LEFT_ENC_PIN), left_wheel_pulse, RISING);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENC_PIN), right_wheel_pulse, RISING);
  #endif
  
  // initMotorController();
  resetPID();
  
#endif

/* Enter the main loop.  Read and parse input from the serial port
   and run any valid commands. Run a PID calculation at the target
   interval and check for auto-stop conditions.
*/
void loop() {
  while (Serial.available() > 0) {
    
    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == 13) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      runCommand();
      resetCommand();
    }
    // Use spaces to delimit parts of the command
    else if (chr == ' ') {
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = NULL;
        arg = 2;
        index = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1) {
        // Subsequent arguments can be more than one character
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }
  
// If we are using base control, run a PID calculation at the appropriate intervals
#ifdef USE_BASE
  if (millis() > nextPID) {
    updatePID();
    nextPID += PID_INTERVAL;
  }
  
  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {;
    setMotorSpeeds(0, 0);
    moving = 0;
  }
#endif

}
