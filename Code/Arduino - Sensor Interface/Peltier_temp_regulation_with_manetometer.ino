 //Cora Ferguson
//Credit: Cam Brochu, controlling the peltier with serial inputs

//imports
#include <math.h>
#include <Adafruit_MMC56x3.h>

//Variables
int baseline;
float temp = 0;                  //this is the temp read in from the user
int powerPin = 5;                //this will output power to the peltier cooler 
int dtime = 1000;                //sets the delay to 1 sec
int input;
const int ThermistorPin = 0;     // analog input pin for the thermistor voltage
int Vo;                          // integer value of voltage reading
float R = 9870.0;                // fixed resistance in the voltage divider
float Rt;                        // computed resistance of the thermistor
int voltage[3];                  //making an array to store 3 values at a time
int x;                           //the integer that will refer to the part of the array that values will be put into
int avg_voltage;                 //the integer that is the average of the 3 voltage readings from the thermistor
Adafruit_MMC5603 mmc = Adafruit_MMC5603(12345); //Assign a unique ID to the magnetic sensor

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  Setup();
  //baseline = getBaseline();
  //Serial.print("Room Temp set to:");
  //Serial.print(baseline);
  //Serial.println("*C");
  //printInstruction();
  
// Only have the arduino calibrate to the original temperature once
  //readInput();
  //checkOutOfRange(input);
  Serial.print("Heating peltier up for 8 seconds");
  //Serial.println(input);
  int output = 170;
  analogWrite(powerPin, output);
  delay(8000);
  
  Serial.println("Adafruit_MMC5603 Magnetometer Test");
  Serial.println("");

  /* Initialise the sensor */
  if (!mmc.begin(MMC56X3_DEFAULT_ADDRESS, &Wire)) {  // I2C mode
    /* There was a problem detecting the MMC5603 ... check your connections */
    Serial.println("Ooops, no MMC5603 detected ... Check your wiring!");
    while (1) delay(10);
  }

  /* Display some basic information on this sensor */
  mmc.printSensorDetails();
}

// Use the loop to continuously monitor the thermistor values and adjust the power output accordingly
void loop() {
  // Push the values into the voltage array to calculate the average voltage from the thermistor
  for (int i = 0; i < 3; i++){
    x = i;
    Vo = int(analogRead(ThermistorPin));
    Rt = R*(1023.0/float(Vo)-1.0);
    voltage[x] = Vo;
    delay(1000);
  }
  
  avg_voltage = (voltage[0]+voltage[1]+voltage[2])/3;
  Serial.print("Average Voltage: ");
  Serial.println(avg_voltage);

  if(avg_voltage > 120){
    analogWrite(powerPin, 0);
  }else if(avg_voltage < 120){
    int output = 90;
    analogWrite(powerPin, output);
  }
  
  // Get a new magnetic sensor event 
  sensors_event_t event;
  mmc.getEvent(&event);

  // Display the results (magnetic vector values are in micro-Tesla (uT))
  Serial.print("X: ");
  Serial.print(event.magnetic.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(event.magnetic.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(event.magnetic.z);
  Serial.print("  ");
  Serial.println("uT");
  delay(3000);
  
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

//Custom functions:
void Setup(){
  pinMode(powerPin, OUTPUT); //setting the powerpin to output mode
  Serial.begin(115200); //starting the serial moniter
}

float getBaseline(){//gathering baseline room temp:
  Serial.println("What is the baseline temperature of the peltier?");
  while (Serial.available() == 0){
  }
  String Baseline = Serial.readString();
  int base = Baseline.toInt();
  return base;
}

void printInstruction(){
  Serial.println("input your desired temperature, then hit enter");
}

void readInput(){
  while (Serial.available() <= 0){
  }
  String Input = Serial.readString();
  input = Input.toInt();
}

void checkOutOfRange(int IN){
  if (IN > 42 || IN < 15){
    Serial.print("Out of range, done testing. Temp is: ");
    Serial.println(temp);
    digitalWrite(powerPin, LOW);
  }
}

//Calibration for Peltier 1
int TempToAnalogOut(int IN){
  int OUT;
  if (IN > baseline+1){
    IN = IN - (baseline - 29.6289); //this reassigns the IN to be the initial value minus the value calculated from the best fit equation
    OUT = log(IN/51.2578) / -0.002; //logic backcalculates the bit using the exponential line of best fit equation
  }else if (IN < baseline-1){
    IN = IN - (baseline - 25.88);
    OUT = log(IN / 25.88) / -0.002; //exponential option
  }else{
    OUT = 0;
  }
  Serial.print("output: ");
  Serial.println(OUT);
  if (OUT > 255){
    OUT = 255;
    Serial.println("corrected to 255");
  }
  return OUT;
}
