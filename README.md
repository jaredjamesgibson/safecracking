# safecracking
Arduino and Python code. 3D .stl files and design. 


The SafeCrackingBruteForceChunks.ino file is the Arduino (C++) code. 

Here are the hardware requirements:
    Servo (5v): 
      - Connected to Pin 10 
    Stepper Motor:
      - Connects to Arduino Motor Shield. 
      - Input DC power is 9v
    Accelerometer (3.3v):
      - Connects to analog pins A1, A2, and A3.
    LED (optional)
      - Connects to Pin 9

Code Sumamry: 

A MASSIVE ASSUMPTION IS THAT YOU KNOW AT LEAST 1 OF THE NUMBERS!!!
    - This is critical because it's the difference between 1M combos (100^3, assuming 3 wheels) and 10k (100^2).
    - In my project, I'm assuming that one of the numbers is "40", but I don't know if it's wheel 1, 2, or 3; hence 10k x 3 = 30k combinations

The python code simply produces that list of 30k numbers into a csv that is then transferred to the Excel file for final form: executeFunction(a,b,c,d,e,f,g,h)
    - If you know 1 of the numbers, swap it out with the 40
    - If you don't know 1 of the numbers, then may God have mercy on your soul. You're in for a long, long ride.

