#include <Stepper.h>              // Main motor to turn the dial
#include <Servo.h>                // Servo to turn the handle after trying a combination  

// Stepper Motor definitions and variables
#define pwmA 3
#define pwmB 11
#define brakeA 9
#define brakeB 8
#define dirA 12
#define dirB 13

// Define the servo pin:
#define servoPin 10             // Servo Pin 10

// ADXL335 Accelerometer
// VCC - 5V
const int xPin = A1;
const int yPin = A2;
const int zPin = A3;
int y = analogRead(yPin);                               // Accelerometer
float yG = map(y, 0, 1023, -100, 100);                  // Accelerometer   

// Define the variables for the Stepper
bool motorHolding = false;                              // Stepper hold state
const int stepsPerRevolution = 200;
int stepperSpeed = 60;                                    // Default speed
Stepper myStepper = Stepper(stepsPerRevolution, dirA, dirB);
int currentPosition = 0; // Variable to store the current position

// Create a variable to store the servo position:
int servoAngle = 45;                                      // Between 30 and 60 for testing
Servo myservo;
 
// Create other variables for the program
int i = 0;  // Step Counter
const int ledPin = 9;                                     // LED connected to digital pin 9
//const int potPin = A3;                                    // Potentiometer connected to analog pin A3 (not used)

// Sets up the main program. Do not make changes here.
void setup() {
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(brakeA, OUTPUT);
  pinMode(brakeB, OUTPUT);

  digitalWrite(pwmA, HIGH);
  digitalWrite(pwmB, HIGH);
  digitalWrite(brakeA, LOW);
  digitalWrite(brakeB, LOW);
  
  myservo.attach(servoPin);
  Serial.begin(9600);         // Initialize serial communication

  // SD Card section; Not used for "Chunks". An SD card is needed to loop through a csv or .txt for more than >800 combos or so...
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
//  Serial.print("Initializing SD card...");
//  if (!SD.begin(4)) {
//    Serial.println("initialization failed!");
//    while (1);
//  }
//  Serial.println("initialization done.");   
  
}

// Primary Execution of a "try" at the same. Prints the combo used and the associated accelerometer value.
void executeFunction(int a, int b, int c, int d, int e, int f, int g, int h) {

  // Set the holding state to true
  motorHolding = true;
  
  // Dial Spinner section
  myStepper.step(a);                  // This is the "Reset" spin, 4 turns to the left. At least 800 -- ADD plus whatever is needed to get it back to true 0 in step 'b'.
  // Reset the position at TDC (top-dead-center)
  int currentPosition = 0;   
  //currentPosition += a;
  delay(1000);                        // Break for a second to let the gears settle.
  myStepper.step(b);                  // This continues the rotation to get to the true first number on the dial. (e.g. Spins 800 in step(a) then an additional 160 to go to "80" on the dial  
  currentPosition += b;
  delay(1000);                        // Break for a second to let the gears settle.  
  myStepper.step(c);                  // This goes to the right around (-200) one time (passes number) then spins to second number
  currentPosition += c;
  delay(1000);                        // Break for a second to let the gears settle.  
  myStepper.step(d);                  // Goes to the third number
  currentPosition += d;
  delay(1000);                        // Break for a second to let the gears settle.  
 
  // Servo Section. Move the servo to turn the handle, and turn the LED on to indicate this action.             
  myservo.write(servoAngle);          // This turns the servo handle (i.e. tests the handle to open)
  analogWrite(ledPin, 255);           // Set to 255 to turn on LED; Placeholder for Actuator or Servo motor.    
  delay(2000);                        // Delay 2 seconds to account for the turn in the servo

  // Set the holding state to false
  motorHolding = false;

  // This simply counts the number of iterations (i) in the sequence
  Serial.print(F(" | Instance: "));      
  Serial.print(e);                    // This should count (i+1) for each instance and print the count -- add actual combo here too (not just stepper ints).  

  // Print the Stepper code that is  being run
  Serial.print(F(" | Stepper Code: "));      
  Serial.print(b);
  Serial.print(F(","));
  Serial.print(c);
  Serial.print(F(","));
  Serial.print(d);

  // Print the combination that is being tested (e.g. 5-29-83)
  Serial.print(F(" | Combination: "));      
  Serial.print(f);
  Serial.print("-");
  Serial.print(g);
  Serial.print("-");
  Serial.print(h);         

  // Print Stepper's Current Position
  Serial.print(F(" | Stepper position: "));      
  Serial.print(currentPosition); 

  // Print out the Accelerometer value at the bottom of the handle pull
  Serial.print(F(" | Value: "));             
  int y = analogRead(yPin);                           // Accelerometer
  float yG = map(y, 0, 1023, -100, 100);              // Accelerometer    
  Serial.println(y);                                  // Final Value: Accelerometer Value
  delay(1000);                                        // Wait for shit to cool

  // Bring the servo back to the original position (top-dead-center) 
  myservo.write(-servoAngle);         // NOTE: Back to the top (90)  
  delay(1000);                        // Delay 2 seconds to account for the turn (back) in the servo
  analogWrite(ledPin, 0);             // This should always be 0 to turn off LED  
 
}

// Stepper Adjustment Section. Use to calibrate and return to TDC (Top-Dead-Center, or "0")
void StepperMotorOneNotch() {
  myStepper.step(2);        // step(2) will move the dial 1 (one) notch.
}
void StepperMotorTenNotch() {
  myStepper.step(20);        // step(20) will move the dial 10 notches.
}

// Test Precision; Rotates on "0". Used in early stepper calibration.
//void TestCalibration() {
//  // Brute force with unadjusted logic  
//  executeFunction(800, -600, 600, -800, 1,0,0,0);
//  executeFunction(800, -600, 600, -800, 2,0,0,0);
//  executeFunction(800, -300, 300, -800, 3,0,0,0);
//  executeFunction(800, -600, 600, -800, 4,0,0,0);
//  executeFunction(800, -600, 600, -800, 5,0,0,0);
//}

// Test Precision; Rotates on "0"; I need this because each time the code recompiles my servo turns (unprompted) 90 degrees. 
// I have to run this "purge" to get things back in order.
void TestCalibrationLite() {
  // Brute force with unadjusted logic  
  executeFunction(400, -200, 400, 400, 0,0,0,0);
}


// Rotates to the kids and hannah's birthdays for p.o.c.; Use known birthdays.
//void TestBirthdays() {
//  // Brute force with adjusted logic; not yet with proper labels
//  executeFunction(0,10,-352,108,1,5,29,83);
//  executeFunction(834,14,-400,24,2,7,7,19);
//  executeFunction(962,16,-390,6,3,8,13,16);
//  executeFunction(968,6,-358,50,4,3,24,49);
//  executeFunction(902,20,-206,84,5,10,7,49);
//  executeFunction(902,6,-394,150,6,3,6,81);
//  executeFunction(838,4,-350,104,7,2,27,79);
//  executeFunction(842,16,-362,106,8,8,27,80);
//  executeFunction(840,6,-378,132,9,3,14,80);
//  executeFunction(840,24,-374,150,10,12,25,0);
//}


// PASTE BRUTE FORCE LOGIC HERE
void BruteForceRun() {
  // MAKE SURE TO DISCONNECT THE HANDLE BEFORE UPLOADING
  // Copy/Paste from Excel file.
  // Set first instance to 0
  executeFunction(0,80,-216,16,11201,40,32,40);
  executeFunction(920,80,-216,18,11202,40,32,41);
  executeFunction(918,80,-216,20,11203,40,32,42);
  executeFunction(916,80,-216,22,11204,40,32,43);
  executeFunction(914,80,-216,24,11205,40,32,44);
  executeFunction(912,80,-216,26,11206,40,32,45);
  executeFunction(910,80,-216,28,11207,40,32,46);
  // ... and it goes on like this.
  // See included Excel file for concatenation of numbers into final executeFunction() form. 

}


// VOID LOOP SECTION 
void loop() {  
  myStepper.setSpeed(stepperSpeed);                 // Set speed; 150 seems to be the max -- 200 is too much. 
  analogWrite(ledPin, 0);                           // Turn the LED light off for starter

if (motorHolding) {
  // Keep the motor in the holding state
  myStepper.step(0);
  return;
}


  // Read user input from serial monitor; series of case when statements.
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input == "run program") {
      BruteForceRun();
    }
    else if (input == "stepper up 1") {
      //Serial.println(F("Stepper motor rotated 1 notch!")); // print a message to the serial monitor
      StepperMotorOneNotch();
    }       
    else if (input == "stepper up 10") {
      //Serial.println(F("Stepper motor rotated 10 notches!")); // print a message to the serial monitor
      StepperMotorTenNotch();
    }           
//    else if (input == "test calibration") {
//      Serial.println(F("Hope it stays at zero, brah!")); // print a message to the serial monitor
//      TestCalibration();
//    } 
    else if (input == "test calibration lite") {
      //Serial.println(F("Zero calories, brah! Shred it!")); // print a message to the serial monitor
      TestCalibrationLite();
    }   
//    else if (input == "test birthdays") {
//      Serial.println(F("Awww! What a family man!")); // print a message to the serial monitor
//      TestBirthdays();
//    }      
    else if (input == "standard reset") {
      Serial.println("You started at zero, right?"); // print a message to the serial monitor
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorTenNotch();
      StepperMotorOneNotch();
      TestCalibrationLite();
    }                           
//    else if (input == "stop program") {
//      Serial.println("Program stopped and reset!"); // print a message to the serial monitor
//      return;
//    }   
  }



}
