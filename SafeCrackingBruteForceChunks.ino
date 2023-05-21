#include <Stepper.h>              // Main motor to turn the dial
#include <Servo.h>                // Servo to turn the handle after trying a combination  

#include <SPI.h>                  // For SD Card             
#include <SD.h>                   // For SD Card 
File myFile;                      // For SD Card 

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

  // SD Card section; Comment out if not working
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


// Stepper Adjustment Section.
void StepperMotorOneNotch() {
  myStepper.step(2);        // step(2) will move the dial 1 (one) notch.
}
void StepperMotorTenNotch() {
  myStepper.step(20);        // step(20) will move the dial 10 notches.
}


// Test Precision; Rotates on "0"
//void TestCalibration() {
//  // Brute force with unadjusted logic  
//  executeFunction(800, -600, 600, -800, 1,0,0,0);
//  executeFunction(800, -600, 600, -800, 2,0,0,0);
//  executeFunction(800, -300, 300, -800, 3,0,0,0);
//  executeFunction(800, -600, 600, -800, 4,0,0,0);
//  executeFunction(800, -600, 600, -800, 5,0,0,0);
//}

// Test Precision; Rotates on "0"
void TestCalibrationLite() {
  // Brute force with unadjusted logic  
  executeFunction(400, -200, 400, 400, 0,0,0,0);
}


// Rotates to the kids and hannah's birthdays for p.o.c.
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
  // Set first instance to 0
executeFunction(0,80,-216,16,11201,40,32,40);
executeFunction(920,80,-216,18,11202,40,32,41);
executeFunction(918,80,-216,20,11203,40,32,42);
executeFunction(916,80,-216,22,11204,40,32,43);
executeFunction(914,80,-216,24,11205,40,32,44);
executeFunction(912,80,-216,26,11206,40,32,45);
executeFunction(910,80,-216,28,11207,40,32,46);
executeFunction(908,80,-216,30,11208,40,32,47);
executeFunction(906,80,-216,32,11209,40,32,48);
executeFunction(904,80,-216,34,11210,40,32,49);
executeFunction(902,80,-216,36,11211,40,32,50);
executeFunction(900,80,-216,38,11212,40,32,51);
executeFunction(898,80,-216,40,11213,40,32,52);
executeFunction(896,80,-216,42,11214,40,32,53);
executeFunction(894,80,-216,44,11215,40,32,54);
executeFunction(892,80,-216,46,11216,40,32,55);
executeFunction(890,80,-216,48,11217,40,32,56);
executeFunction(888,80,-216,50,11218,40,32,57);
executeFunction(886,80,-216,52,11219,40,32,58);
executeFunction(884,80,-216,54,11220,40,32,59);
executeFunction(882,80,-216,56,11221,40,32,60);
executeFunction(880,80,-216,58,11222,40,32,61);
executeFunction(878,80,-216,60,11223,40,32,62);
executeFunction(876,80,-216,62,11224,40,32,63);
executeFunction(874,80,-216,64,11225,40,32,64);
executeFunction(872,80,-216,66,11226,40,32,65);
executeFunction(870,80,-216,68,11227,40,32,66);
executeFunction(868,80,-216,70,11228,40,32,67);
executeFunction(866,80,-216,72,11229,40,32,68);
executeFunction(864,80,-216,74,11230,40,32,69);
executeFunction(862,80,-216,76,11231,40,32,70);
executeFunction(860,80,-216,78,11232,40,32,71);
executeFunction(858,80,-216,80,11233,40,32,72);
executeFunction(856,80,-216,82,11234,40,32,73);
executeFunction(854,80,-216,84,11235,40,32,74);
executeFunction(852,80,-216,86,11236,40,32,75);
executeFunction(850,80,-216,88,11237,40,32,76);
executeFunction(848,80,-216,90,11238,40,32,77);
executeFunction(846,80,-216,92,11239,40,32,78);
executeFunction(844,80,-216,94,11240,40,32,79);
executeFunction(842,80,-216,96,11241,40,32,80);
executeFunction(840,80,-216,98,11242,40,32,81);
executeFunction(838,80,-216,100,11243,40,32,82);
executeFunction(836,80,-216,102,11244,40,32,83);
executeFunction(834,80,-216,104,11245,40,32,84);
executeFunction(832,80,-216,106,11246,40,32,85);
executeFunction(830,80,-216,108,11247,40,32,86);
executeFunction(828,80,-216,110,11248,40,32,87);
executeFunction(826,80,-216,112,11249,40,32,88);
executeFunction(824,80,-216,114,11250,40,32,89);
executeFunction(822,80,-216,116,11251,40,32,90);
executeFunction(820,80,-216,118,11252,40,32,91);
executeFunction(818,80,-216,120,11253,40,32,92);
executeFunction(816,80,-216,122,11254,40,32,93);
executeFunction(814,80,-216,124,11255,40,32,94);
executeFunction(812,80,-216,126,11256,40,32,95);
executeFunction(810,80,-216,128,11257,40,32,96);
executeFunction(808,80,-216,130,11258,40,32,97);
executeFunction(806,80,-216,132,11259,40,32,98);
executeFunction(804,80,-216,134,11260,40,32,99);
executeFunction(802,80,-214,134,11261,40,33,0);
executeFunction(1000,80,-214,136,11262,40,33,1);
executeFunction(998,80,-214,138,11263,40,33,2);
executeFunction(996,80,-214,140,11264,40,33,3);
executeFunction(994,80,-214,142,11265,40,33,4);
executeFunction(992,80,-214,144,11266,40,33,5);
executeFunction(990,80,-214,146,11267,40,33,6);
executeFunction(988,80,-214,148,11268,40,33,7);
executeFunction(986,80,-214,150,11269,40,33,8);
executeFunction(984,80,-214,152,11270,40,33,9);
executeFunction(982,80,-214,154,11271,40,33,10);
executeFunction(980,80,-214,156,11272,40,33,11);
executeFunction(978,80,-214,158,11273,40,33,12);
executeFunction(976,80,-214,160,11274,40,33,13);
executeFunction(974,80,-214,162,11275,40,33,14);
executeFunction(972,80,-214,164,11276,40,33,15);
executeFunction(970,80,-214,166,11277,40,33,16);
executeFunction(968,80,-214,168,11278,40,33,17);
executeFunction(966,80,-214,170,11279,40,33,18);
executeFunction(964,80,-214,172,11280,40,33,19);
executeFunction(962,80,-214,174,11281,40,33,20);
executeFunction(960,80,-214,176,11282,40,33,21);
executeFunction(958,80,-214,178,11283,40,33,22);
executeFunction(956,80,-214,180,11284,40,33,23);
executeFunction(954,80,-214,182,11285,40,33,24);
executeFunction(952,80,-214,184,11286,40,33,25);
executeFunction(950,80,-214,186,11287,40,33,26);
executeFunction(948,80,-214,188,11288,40,33,27);
executeFunction(946,80,-214,190,11289,40,33,28);
executeFunction(944,80,-214,192,11290,40,33,29);
executeFunction(942,80,-214,194,11291,40,33,30);
executeFunction(940,80,-214,196,11292,40,33,31);
executeFunction(938,80,-214,198,11293,40,33,32);
executeFunction(936,80,-214,200,11294,40,33,33);
executeFunction(934,80,-214,2,11295,40,33,34);
executeFunction(932,80,-214,4,11296,40,33,35);
executeFunction(930,80,-214,6,11297,40,33,36);
executeFunction(928,80,-214,8,11298,40,33,37);
executeFunction(926,80,-214,10,11299,40,33,38);
executeFunction(924,80,-214,12,11300,40,33,39);
executeFunction(922,80,-214,14,11301,40,33,40);
executeFunction(920,80,-214,16,11302,40,33,41);
executeFunction(918,80,-214,18,11303,40,33,42);
executeFunction(916,80,-214,20,11304,40,33,43);
executeFunction(914,80,-214,22,11305,40,33,44);
executeFunction(912,80,-214,24,11306,40,33,45);
executeFunction(910,80,-214,26,11307,40,33,46);
executeFunction(908,80,-214,28,11308,40,33,47);
executeFunction(906,80,-214,30,11309,40,33,48);
executeFunction(904,80,-214,32,11310,40,33,49);
executeFunction(902,80,-214,34,11311,40,33,50);
executeFunction(900,80,-214,36,11312,40,33,51);
executeFunction(898,80,-214,38,11313,40,33,52);
executeFunction(896,80,-214,40,11314,40,33,53);
executeFunction(894,80,-214,42,11315,40,33,54);
executeFunction(892,80,-214,44,11316,40,33,55);
executeFunction(890,80,-214,46,11317,40,33,56);
executeFunction(888,80,-214,48,11318,40,33,57);
executeFunction(886,80,-214,50,11319,40,33,58);
executeFunction(884,80,-214,52,11320,40,33,59);
executeFunction(882,80,-214,54,11321,40,33,60);
executeFunction(880,80,-214,56,11322,40,33,61);
executeFunction(878,80,-214,58,11323,40,33,62);
executeFunction(876,80,-214,60,11324,40,33,63);
executeFunction(874,80,-214,62,11325,40,33,64);
executeFunction(872,80,-214,64,11326,40,33,65);
executeFunction(870,80,-214,66,11327,40,33,66);
executeFunction(868,80,-214,68,11328,40,33,67);
executeFunction(866,80,-214,70,11329,40,33,68);
executeFunction(864,80,-214,72,11330,40,33,69);
executeFunction(862,80,-214,74,11331,40,33,70);
executeFunction(860,80,-214,76,11332,40,33,71);
executeFunction(858,80,-214,78,11333,40,33,72);
executeFunction(856,80,-214,80,11334,40,33,73);
executeFunction(854,80,-214,82,11335,40,33,74);
executeFunction(852,80,-214,84,11336,40,33,75);
executeFunction(850,80,-214,86,11337,40,33,76);
executeFunction(848,80,-214,88,11338,40,33,77);
executeFunction(846,80,-214,90,11339,40,33,78);
executeFunction(844,80,-214,92,11340,40,33,79);
executeFunction(842,80,-214,94,11341,40,33,80);
executeFunction(840,80,-214,96,11342,40,33,81);
executeFunction(838,80,-214,98,11343,40,33,82);
executeFunction(836,80,-214,100,11344,40,33,83);
executeFunction(834,80,-214,102,11345,40,33,84);
executeFunction(832,80,-214,104,11346,40,33,85);
executeFunction(830,80,-214,106,11347,40,33,86);
executeFunction(828,80,-214,108,11348,40,33,87);
executeFunction(826,80,-214,110,11349,40,33,88);
executeFunction(824,80,-214,112,11350,40,33,89);
executeFunction(822,80,-214,114,11351,40,33,90);
executeFunction(820,80,-214,116,11352,40,33,91);
executeFunction(818,80,-214,118,11353,40,33,92);
executeFunction(816,80,-214,120,11354,40,33,93);
executeFunction(814,80,-214,122,11355,40,33,94);
executeFunction(812,80,-214,124,11356,40,33,95);
executeFunction(810,80,-214,126,11357,40,33,96);
executeFunction(808,80,-214,128,11358,40,33,97);
executeFunction(806,80,-214,130,11359,40,33,98);
executeFunction(804,80,-214,132,11360,40,33,99);
executeFunction(802,80,-212,132,11361,40,34,0);
executeFunction(1000,80,-212,134,11362,40,34,1);
executeFunction(998,80,-212,136,11363,40,34,2);
executeFunction(996,80,-212,138,11364,40,34,3);
executeFunction(994,80,-212,140,11365,40,34,4);
executeFunction(992,80,-212,142,11366,40,34,5);
executeFunction(990,80,-212,144,11367,40,34,6);
executeFunction(988,80,-212,146,11368,40,34,7);
executeFunction(986,80,-212,148,11369,40,34,8);
executeFunction(984,80,-212,150,11370,40,34,9);
executeFunction(982,80,-212,152,11371,40,34,10);
executeFunction(980,80,-212,154,11372,40,34,11);
executeFunction(978,80,-212,156,11373,40,34,12);
executeFunction(976,80,-212,158,11374,40,34,13);
executeFunction(974,80,-212,160,11375,40,34,14);
executeFunction(972,80,-212,162,11376,40,34,15);
executeFunction(970,80,-212,164,11377,40,34,16);
executeFunction(968,80,-212,166,11378,40,34,17);
executeFunction(966,80,-212,168,11379,40,34,18);
executeFunction(964,80,-212,170,11380,40,34,19);
executeFunction(962,80,-212,172,11381,40,34,20);
executeFunction(960,80,-212,174,11382,40,34,21);
executeFunction(958,80,-212,176,11383,40,34,22);
executeFunction(956,80,-212,178,11384,40,34,23);
executeFunction(954,80,-212,180,11385,40,34,24);
executeFunction(952,80,-212,182,11386,40,34,25);
executeFunction(950,80,-212,184,11387,40,34,26);
executeFunction(948,80,-212,186,11388,40,34,27);
executeFunction(946,80,-212,188,11389,40,34,28);
executeFunction(944,80,-212,190,11390,40,34,29);
executeFunction(942,80,-212,192,11391,40,34,30);
executeFunction(940,80,-212,194,11392,40,34,31);
executeFunction(938,80,-212,196,11393,40,34,32);
executeFunction(936,80,-212,198,11394,40,34,33);
executeFunction(934,80,-212,200,11395,40,34,34);
executeFunction(932,80,-212,2,11396,40,34,35);
executeFunction(930,80,-212,4,11397,40,34,36);
executeFunction(928,80,-212,6,11398,40,34,37);
executeFunction(926,80,-212,8,11399,40,34,38);
executeFunction(924,80,-212,10,11400,40,34,39);
executeFunction(922,80,-212,12,11401,40,34,40);
executeFunction(920,80,-212,14,11402,40,34,41);
executeFunction(918,80,-212,16,11403,40,34,42);
executeFunction(916,80,-212,18,11404,40,34,43);
executeFunction(914,80,-212,20,11405,40,34,44);
executeFunction(912,80,-212,22,11406,40,34,45);
executeFunction(910,80,-212,24,11407,40,34,46);
executeFunction(908,80,-212,26,11408,40,34,47);
executeFunction(906,80,-212,28,11409,40,34,48);
executeFunction(904,80,-212,30,11410,40,34,49);
executeFunction(902,80,-212,32,11411,40,34,50);
executeFunction(900,80,-212,34,11412,40,34,51);
executeFunction(898,80,-212,36,11413,40,34,52);
executeFunction(896,80,-212,38,11414,40,34,53);
executeFunction(894,80,-212,40,11415,40,34,54);
executeFunction(892,80,-212,42,11416,40,34,55);
executeFunction(890,80,-212,44,11417,40,34,56);
executeFunction(888,80,-212,46,11418,40,34,57);
executeFunction(886,80,-212,48,11419,40,34,58);
executeFunction(884,80,-212,50,11420,40,34,59);
executeFunction(882,80,-212,52,11421,40,34,60);
executeFunction(880,80,-212,54,11422,40,34,61);
executeFunction(878,80,-212,56,11423,40,34,62);
executeFunction(876,80,-212,58,11424,40,34,63);
executeFunction(874,80,-212,60,11425,40,34,64);
executeFunction(872,80,-212,62,11426,40,34,65);
executeFunction(870,80,-212,64,11427,40,34,66);
executeFunction(868,80,-212,66,11428,40,34,67);
executeFunction(866,80,-212,68,11429,40,34,68);
executeFunction(864,80,-212,70,11430,40,34,69);
executeFunction(862,80,-212,72,11431,40,34,70);
executeFunction(860,80,-212,74,11432,40,34,71);
executeFunction(858,80,-212,76,11433,40,34,72);
executeFunction(856,80,-212,78,11434,40,34,73);
executeFunction(854,80,-212,80,11435,40,34,74);
executeFunction(852,80,-212,82,11436,40,34,75);
executeFunction(850,80,-212,84,11437,40,34,76);
executeFunction(848,80,-212,86,11438,40,34,77);
executeFunction(846,80,-212,88,11439,40,34,78);
executeFunction(844,80,-212,90,11440,40,34,79);
executeFunction(842,80,-212,92,11441,40,34,80);
executeFunction(840,80,-212,94,11442,40,34,81);
executeFunction(838,80,-212,96,11443,40,34,82);
executeFunction(836,80,-212,98,11444,40,34,83);
executeFunction(834,80,-212,100,11445,40,34,84);
executeFunction(832,80,-212,102,11446,40,34,85);
executeFunction(830,80,-212,104,11447,40,34,86);
executeFunction(828,80,-212,106,11448,40,34,87);
executeFunction(826,80,-212,108,11449,40,34,88);
executeFunction(824,80,-212,110,11450,40,34,89);
executeFunction(822,80,-212,112,11451,40,34,90);
executeFunction(820,80,-212,114,11452,40,34,91);
executeFunction(818,80,-212,116,11453,40,34,92);
executeFunction(816,80,-212,118,11454,40,34,93);
executeFunction(814,80,-212,120,11455,40,34,94);
executeFunction(812,80,-212,122,11456,40,34,95);
executeFunction(810,80,-212,124,11457,40,34,96);
executeFunction(808,80,-212,126,11458,40,34,97);
executeFunction(806,80,-212,128,11459,40,34,98);
executeFunction(804,80,-212,130,11460,40,34,99);
executeFunction(802,80,-210,130,11461,40,35,0);
executeFunction(1000,80,-210,132,11462,40,35,1);
executeFunction(998,80,-210,134,11463,40,35,2);
executeFunction(996,80,-210,136,11464,40,35,3);
executeFunction(994,80,-210,138,11465,40,35,4);
executeFunction(992,80,-210,140,11466,40,35,5);
executeFunction(990,80,-210,142,11467,40,35,6);
executeFunction(988,80,-210,144,11468,40,35,7);
executeFunction(986,80,-210,146,11469,40,35,8);
executeFunction(984,80,-210,148,11470,40,35,9);
executeFunction(982,80,-210,150,11471,40,35,10);
executeFunction(980,80,-210,152,11472,40,35,11);
executeFunction(978,80,-210,154,11473,40,35,12);
executeFunction(976,80,-210,156,11474,40,35,13);
executeFunction(974,80,-210,158,11475,40,35,14);
executeFunction(972,80,-210,160,11476,40,35,15);
executeFunction(970,80,-210,162,11477,40,35,16);
executeFunction(968,80,-210,164,11478,40,35,17);
executeFunction(966,80,-210,166,11479,40,35,18);
executeFunction(964,80,-210,168,11480,40,35,19);
executeFunction(962,80,-210,170,11481,40,35,20);
executeFunction(960,80,-210,172,11482,40,35,21);
executeFunction(958,80,-210,174,11483,40,35,22);
executeFunction(956,80,-210,176,11484,40,35,23);
executeFunction(954,80,-210,178,11485,40,35,24);
executeFunction(952,80,-210,180,11486,40,35,25);
executeFunction(950,80,-210,182,11487,40,35,26);
executeFunction(948,80,-210,184,11488,40,35,27);
executeFunction(946,80,-210,186,11489,40,35,28);
executeFunction(944,80,-210,188,11490,40,35,29);
executeFunction(942,80,-210,190,11491,40,35,30);
executeFunction(940,80,-210,192,11492,40,35,31);
executeFunction(938,80,-210,194,11493,40,35,32);
executeFunction(936,80,-210,196,11494,40,35,33);
executeFunction(934,80,-210,198,11495,40,35,34);
executeFunction(932,80,-210,200,11496,40,35,35);
executeFunction(930,80,-210,2,11497,40,35,36);
executeFunction(928,80,-210,4,11498,40,35,37);
executeFunction(926,80,-210,6,11499,40,35,38);
executeFunction(924,80,-210,8,11500,40,35,39);
executeFunction(922,80,-210,10,11501,40,35,40);
executeFunction(920,80,-210,12,11502,40,35,41);
executeFunction(918,80,-210,14,11503,40,35,42);
executeFunction(916,80,-210,16,11504,40,35,43);
executeFunction(914,80,-210,18,11505,40,35,44);
executeFunction(912,80,-210,20,11506,40,35,45);
executeFunction(910,80,-210,22,11507,40,35,46);
executeFunction(908,80,-210,24,11508,40,35,47);
executeFunction(906,80,-210,26,11509,40,35,48);
executeFunction(904,80,-210,28,11510,40,35,49);
executeFunction(902,80,-210,30,11511,40,35,50);
executeFunction(900,80,-210,32,11512,40,35,51);
executeFunction(898,80,-210,34,11513,40,35,52);
executeFunction(896,80,-210,36,11514,40,35,53);
executeFunction(894,80,-210,38,11515,40,35,54);
executeFunction(892,80,-210,40,11516,40,35,55);
executeFunction(890,80,-210,42,11517,40,35,56);
executeFunction(888,80,-210,44,11518,40,35,57);
executeFunction(886,80,-210,46,11519,40,35,58);
executeFunction(884,80,-210,48,11520,40,35,59);
executeFunction(882,80,-210,50,11521,40,35,60);
executeFunction(880,80,-210,52,11522,40,35,61);
executeFunction(878,80,-210,54,11523,40,35,62);
executeFunction(876,80,-210,56,11524,40,35,63);
executeFunction(874,80,-210,58,11525,40,35,64);
executeFunction(872,80,-210,60,11526,40,35,65);
executeFunction(870,80,-210,62,11527,40,35,66);
executeFunction(868,80,-210,64,11528,40,35,67);
executeFunction(866,80,-210,66,11529,40,35,68);
executeFunction(864,80,-210,68,11530,40,35,69);
executeFunction(862,80,-210,70,11531,40,35,70);
executeFunction(860,80,-210,72,11532,40,35,71);
executeFunction(858,80,-210,74,11533,40,35,72);
executeFunction(856,80,-210,76,11534,40,35,73);
executeFunction(854,80,-210,78,11535,40,35,74);
executeFunction(852,80,-210,80,11536,40,35,75);
executeFunction(850,80,-210,82,11537,40,35,76);
executeFunction(848,80,-210,84,11538,40,35,77);
executeFunction(846,80,-210,86,11539,40,35,78);
executeFunction(844,80,-210,88,11540,40,35,79);
executeFunction(842,80,-210,90,11541,40,35,80);
executeFunction(840,80,-210,92,11542,40,35,81);
executeFunction(838,80,-210,94,11543,40,35,82);
executeFunction(836,80,-210,96,11544,40,35,83);
executeFunction(834,80,-210,98,11545,40,35,84);
executeFunction(832,80,-210,100,11546,40,35,85);
executeFunction(830,80,-210,102,11547,40,35,86);
executeFunction(828,80,-210,104,11548,40,35,87);
executeFunction(826,80,-210,106,11549,40,35,88);
executeFunction(824,80,-210,108,11550,40,35,89);
executeFunction(822,80,-210,110,11551,40,35,90);
executeFunction(820,80,-210,112,11552,40,35,91);
executeFunction(818,80,-210,114,11553,40,35,92);
executeFunction(816,80,-210,116,11554,40,35,93);
executeFunction(814,80,-210,118,11555,40,35,94);
executeFunction(812,80,-210,120,11556,40,35,95);
executeFunction(810,80,-210,122,11557,40,35,96);
executeFunction(808,80,-210,124,11558,40,35,97);
executeFunction(806,80,-210,126,11559,40,35,98);
executeFunction(804,80,-210,128,11560,40,35,99);
executeFunction(802,80,-208,128,11561,40,36,0);
executeFunction(1000,80,-208,130,11562,40,36,1);
executeFunction(998,80,-208,132,11563,40,36,2);
executeFunction(996,80,-208,134,11564,40,36,3);
executeFunction(994,80,-208,136,11565,40,36,4);
executeFunction(992,80,-208,138,11566,40,36,5);
executeFunction(990,80,-208,140,11567,40,36,6);
executeFunction(988,80,-208,142,11568,40,36,7);
executeFunction(986,80,-208,144,11569,40,36,8);
executeFunction(984,80,-208,146,11570,40,36,9);
executeFunction(982,80,-208,148,11571,40,36,10);
executeFunction(980,80,-208,150,11572,40,36,11);
executeFunction(978,80,-208,152,11573,40,36,12);
executeFunction(976,80,-208,154,11574,40,36,13);
executeFunction(974,80,-208,156,11575,40,36,14);
executeFunction(972,80,-208,158,11576,40,36,15);
executeFunction(970,80,-208,160,11577,40,36,16);
executeFunction(968,80,-208,162,11578,40,36,17);
executeFunction(966,80,-208,164,11579,40,36,18);
executeFunction(964,80,-208,166,11580,40,36,19);
executeFunction(962,80,-208,168,11581,40,36,20);
executeFunction(960,80,-208,170,11582,40,36,21);
executeFunction(958,80,-208,172,11583,40,36,22);
executeFunction(956,80,-208,174,11584,40,36,23);
executeFunction(954,80,-208,176,11585,40,36,24);
executeFunction(952,80,-208,178,11586,40,36,25);
executeFunction(950,80,-208,180,11587,40,36,26);
executeFunction(948,80,-208,182,11588,40,36,27);
executeFunction(946,80,-208,184,11589,40,36,28);
executeFunction(944,80,-208,186,11590,40,36,29);
executeFunction(942,80,-208,188,11591,40,36,30);
executeFunction(940,80,-208,190,11592,40,36,31);
executeFunction(938,80,-208,192,11593,40,36,32);
executeFunction(936,80,-208,194,11594,40,36,33);
executeFunction(934,80,-208,196,11595,40,36,34);
executeFunction(932,80,-208,198,11596,40,36,35);
executeFunction(930,80,-208,200,11597,40,36,36);
executeFunction(928,80,-208,2,11598,40,36,37);
executeFunction(926,80,-208,4,11599,40,36,38);
executeFunction(924,80,-208,6,11600,40,36,39);
executeFunction(922,80,-208,8,11601,40,36,40);
executeFunction(920,80,-208,10,11602,40,36,41);
executeFunction(918,80,-208,12,11603,40,36,42);
executeFunction(916,80,-208,14,11604,40,36,43);
executeFunction(914,80,-208,16,11605,40,36,44);
executeFunction(912,80,-208,18,11606,40,36,45);
executeFunction(910,80,-208,20,11607,40,36,46);
executeFunction(908,80,-208,22,11608,40,36,47);
executeFunction(906,80,-208,24,11609,40,36,48);
executeFunction(904,80,-208,26,11610,40,36,49);
executeFunction(902,80,-208,28,11611,40,36,50);
executeFunction(900,80,-208,30,11612,40,36,51);
executeFunction(898,80,-208,32,11613,40,36,52);
executeFunction(896,80,-208,34,11614,40,36,53);
executeFunction(894,80,-208,36,11615,40,36,54);
executeFunction(892,80,-208,38,11616,40,36,55);
executeFunction(890,80,-208,40,11617,40,36,56);
executeFunction(888,80,-208,42,11618,40,36,57);
executeFunction(886,80,-208,44,11619,40,36,58);
executeFunction(884,80,-208,46,11620,40,36,59);
executeFunction(882,80,-208,48,11621,40,36,60);
executeFunction(880,80,-208,50,11622,40,36,61);
executeFunction(878,80,-208,52,11623,40,36,62);
executeFunction(876,80,-208,54,11624,40,36,63);
executeFunction(874,80,-208,56,11625,40,36,64);
executeFunction(872,80,-208,58,11626,40,36,65);
executeFunction(870,80,-208,60,11627,40,36,66);
executeFunction(868,80,-208,62,11628,40,36,67);
executeFunction(866,80,-208,64,11629,40,36,68);
executeFunction(864,80,-208,66,11630,40,36,69);
executeFunction(862,80,-208,68,11631,40,36,70);
executeFunction(860,80,-208,70,11632,40,36,71);
executeFunction(858,80,-208,72,11633,40,36,72);
executeFunction(856,80,-208,74,11634,40,36,73);
executeFunction(854,80,-208,76,11635,40,36,74);
executeFunction(852,80,-208,78,11636,40,36,75);
executeFunction(850,80,-208,80,11637,40,36,76);
executeFunction(848,80,-208,82,11638,40,36,77);
executeFunction(846,80,-208,84,11639,40,36,78);
executeFunction(844,80,-208,86,11640,40,36,79);
executeFunction(842,80,-208,88,11641,40,36,80);
executeFunction(840,80,-208,90,11642,40,36,81);
executeFunction(838,80,-208,92,11643,40,36,82);
executeFunction(836,80,-208,94,11644,40,36,83);
executeFunction(834,80,-208,96,11645,40,36,84);
executeFunction(832,80,-208,98,11646,40,36,85);
executeFunction(830,80,-208,100,11647,40,36,86);
executeFunction(828,80,-208,102,11648,40,36,87);
executeFunction(826,80,-208,104,11649,40,36,88);
executeFunction(824,80,-208,106,11650,40,36,89);
executeFunction(822,80,-208,108,11651,40,36,90);
executeFunction(820,80,-208,110,11652,40,36,91);
executeFunction(818,80,-208,112,11653,40,36,92);
executeFunction(816,80,-208,114,11654,40,36,93);
executeFunction(814,80,-208,116,11655,40,36,94);
executeFunction(812,80,-208,118,11656,40,36,95);
executeFunction(810,80,-208,120,11657,40,36,96);
executeFunction(808,80,-208,122,11658,40,36,97);
executeFunction(806,80,-208,124,11659,40,36,98);
executeFunction(804,80,-208,126,11660,40,36,99);
executeFunction(802,80,-206,126,11661,40,37,0);
executeFunction(1000,80,-206,128,11662,40,37,1);
executeFunction(998,80,-206,130,11663,40,37,2);
executeFunction(996,80,-206,132,11664,40,37,3);
executeFunction(994,80,-206,134,11665,40,37,4);
executeFunction(992,80,-206,136,11666,40,37,5);
executeFunction(990,80,-206,138,11667,40,37,6);
executeFunction(988,80,-206,140,11668,40,37,7);
executeFunction(986,80,-206,142,11669,40,37,8);
executeFunction(984,80,-206,144,11670,40,37,9);
executeFunction(982,80,-206,146,11671,40,37,10);
executeFunction(980,80,-206,148,11672,40,37,11);
executeFunction(978,80,-206,150,11673,40,37,12);
executeFunction(976,80,-206,152,11674,40,37,13);
executeFunction(974,80,-206,154,11675,40,37,14);
executeFunction(972,80,-206,156,11676,40,37,15);
executeFunction(970,80,-206,158,11677,40,37,16);
executeFunction(968,80,-206,160,11678,40,37,17);
executeFunction(966,80,-206,162,11679,40,37,18);
executeFunction(964,80,-206,164,11680,40,37,19);
executeFunction(962,80,-206,166,11681,40,37,20);
executeFunction(960,80,-206,168,11682,40,37,21);
executeFunction(958,80,-206,170,11683,40,37,22);
executeFunction(956,80,-206,172,11684,40,37,23);
executeFunction(954,80,-206,174,11685,40,37,24);
executeFunction(952,80,-206,176,11686,40,37,25);
executeFunction(950,80,-206,178,11687,40,37,26);
executeFunction(948,80,-206,180,11688,40,37,27);
executeFunction(946,80,-206,182,11689,40,37,28);
executeFunction(944,80,-206,184,11690,40,37,29);
executeFunction(942,80,-206,186,11691,40,37,30);
executeFunction(940,80,-206,188,11692,40,37,31);
executeFunction(938,80,-206,190,11693,40,37,32);
executeFunction(936,80,-206,192,11694,40,37,33);
executeFunction(934,80,-206,194,11695,40,37,34);
executeFunction(932,80,-206,196,11696,40,37,35);
executeFunction(930,80,-206,198,11697,40,37,36);
executeFunction(928,80,-206,200,11698,40,37,37);
executeFunction(926,80,-206,2,11699,40,37,38);
executeFunction(924,80,-206,4,11700,40,37,39);
executeFunction(922,80,-206,6,11701,40,37,40);
executeFunction(920,80,-206,8,11702,40,37,41);
executeFunction(918,80,-206,10,11703,40,37,42);
executeFunction(916,80,-206,12,11704,40,37,43);
executeFunction(914,80,-206,14,11705,40,37,44);
executeFunction(912,80,-206,16,11706,40,37,45);
executeFunction(910,80,-206,18,11707,40,37,46);
executeFunction(908,80,-206,20,11708,40,37,47);
executeFunction(906,80,-206,22,11709,40,37,48);
executeFunction(904,80,-206,24,11710,40,37,49);
executeFunction(902,80,-206,26,11711,40,37,50);
executeFunction(900,80,-206,28,11712,40,37,51);
executeFunction(898,80,-206,30,11713,40,37,52);
executeFunction(896,80,-206,32,11714,40,37,53);
executeFunction(894,80,-206,34,11715,40,37,54);
executeFunction(892,80,-206,36,11716,40,37,55);
executeFunction(890,80,-206,38,11717,40,37,56);
executeFunction(888,80,-206,40,11718,40,37,57);
executeFunction(886,80,-206,42,11719,40,37,58);
executeFunction(884,80,-206,44,11720,40,37,59);
executeFunction(882,80,-206,46,11721,40,37,60);
executeFunction(880,80,-206,48,11722,40,37,61);
executeFunction(878,80,-206,50,11723,40,37,62);
executeFunction(876,80,-206,52,11724,40,37,63);
executeFunction(874,80,-206,54,11725,40,37,64);
executeFunction(872,80,-206,56,11726,40,37,65);
executeFunction(870,80,-206,58,11727,40,37,66);
executeFunction(868,80,-206,60,11728,40,37,67);
executeFunction(866,80,-206,62,11729,40,37,68);
executeFunction(864,80,-206,64,11730,40,37,69);
executeFunction(862,80,-206,66,11731,40,37,70);
executeFunction(860,80,-206,68,11732,40,37,71);
executeFunction(858,80,-206,70,11733,40,37,72);
executeFunction(856,80,-206,72,11734,40,37,73);
executeFunction(854,80,-206,74,11735,40,37,74);
executeFunction(852,80,-206,76,11736,40,37,75);
executeFunction(850,80,-206,78,11737,40,37,76);
executeFunction(848,80,-206,80,11738,40,37,77);
executeFunction(846,80,-206,82,11739,40,37,78);
executeFunction(844,80,-206,84,11740,40,37,79);
executeFunction(842,80,-206,86,11741,40,37,80);
executeFunction(840,80,-206,88,11742,40,37,81);
executeFunction(838,80,-206,90,11743,40,37,82);
executeFunction(836,80,-206,92,11744,40,37,83);
executeFunction(834,80,-206,94,11745,40,37,84);
executeFunction(832,80,-206,96,11746,40,37,85);
executeFunction(830,80,-206,98,11747,40,37,86);
executeFunction(828,80,-206,100,11748,40,37,87);
executeFunction(826,80,-206,102,11749,40,37,88);
executeFunction(824,80,-206,104,11750,40,37,89);
executeFunction(822,80,-206,106,11751,40,37,90);
executeFunction(820,80,-206,108,11752,40,37,91);
executeFunction(818,80,-206,110,11753,40,37,92);
executeFunction(816,80,-206,112,11754,40,37,93);
executeFunction(814,80,-206,114,11755,40,37,94);
executeFunction(812,80,-206,116,11756,40,37,95);
executeFunction(810,80,-206,118,11757,40,37,96);
executeFunction(808,80,-206,120,11758,40,37,97);
executeFunction(806,80,-206,122,11759,40,37,98);
executeFunction(804,80,-206,124,11760,40,37,99);
executeFunction(802,80,-204,124,11761,40,38,0);
executeFunction(1000,80,-204,126,11762,40,38,1);
executeFunction(998,80,-204,128,11763,40,38,2);
executeFunction(996,80,-204,130,11764,40,38,3);
executeFunction(994,80,-204,132,11765,40,38,4);
executeFunction(992,80,-204,134,11766,40,38,5);
executeFunction(990,80,-204,136,11767,40,38,6);
executeFunction(988,80,-204,138,11768,40,38,7);
executeFunction(986,80,-204,140,11769,40,38,8);
executeFunction(984,80,-204,142,11770,40,38,9);
executeFunction(982,80,-204,144,11771,40,38,10);
executeFunction(980,80,-204,146,11772,40,38,11);
executeFunction(978,80,-204,148,11773,40,38,12);
executeFunction(976,80,-204,150,11774,40,38,13);
executeFunction(974,80,-204,152,11775,40,38,14);
executeFunction(972,80,-204,154,11776,40,38,15);
executeFunction(970,80,-204,156,11777,40,38,16);
executeFunction(968,80,-204,158,11778,40,38,17);
executeFunction(966,80,-204,160,11779,40,38,18);
executeFunction(964,80,-204,162,11780,40,38,19);
executeFunction(962,80,-204,164,11781,40,38,20);
executeFunction(960,80,-204,166,11782,40,38,21);
executeFunction(958,80,-204,168,11783,40,38,22);
executeFunction(956,80,-204,170,11784,40,38,23);
executeFunction(954,80,-204,172,11785,40,38,24);
executeFunction(952,80,-204,174,11786,40,38,25);
executeFunction(950,80,-204,176,11787,40,38,26);
executeFunction(948,80,-204,178,11788,40,38,27);
executeFunction(946,80,-204,180,11789,40,38,28);
executeFunction(944,80,-204,182,11790,40,38,29);
executeFunction(942,80,-204,184,11791,40,38,30);
executeFunction(940,80,-204,186,11792,40,38,31);
executeFunction(938,80,-204,188,11793,40,38,32);
executeFunction(936,80,-204,190,11794,40,38,33);
executeFunction(934,80,-204,192,11795,40,38,34);
executeFunction(932,80,-204,194,11796,40,38,35);
executeFunction(930,80,-204,196,11797,40,38,36);
executeFunction(928,80,-204,198,11798,40,38,37);
executeFunction(926,80,-204,200,11799,40,38,38);
executeFunction(924,80,-204,2,11800,40,38,39);





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
