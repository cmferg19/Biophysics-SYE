// Cora Ferguson
//Credit: Cameron Brochu
//the purpose of this code is to test the Peltier cooler and create 2 graphs:
//Arduino Output Voltage VS. Hot Side Temperature
//Arduino Output Voltage VS. Cold Side Temperature

//variables:
int PeltierPin = 5; //the PWM digital output channel that will control the Arduino
int power = 0; //initializes output voltage as a percentage to 0%, this can go from 0% to 100%
int change = 0; //status of if the input value has changed
int samples = 0; //array to store the resistance samples
#define ntc_pin A0
#define vd_power_pin 2
#define samplingrate 5
#define Rref 10000
const int ThermistorPin = 0; // analog input pin for the thermistor voltage
int Vo;               // integer value of voltage reading
float R = 9870.0;     // fixed resistance in the voltage divider
float Rt;             // computed resistance of the thermistor
int voltage[3]; //making an array to store 3 values at a time
int x; //the integer that will refer to the part of the array that values will be put into

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //starts the serial monitor
  pinMode(PeltierPin, OUTPUT); //sets the defined PeltierPin to output mode (to send voltage to Peltier)
  Serial.println("Press the space bar, then hit enter, to move to the next output");
  Serial.println("Press the r key, then hit enter, to go back to the previous output");
  Serial.println("Thermistor voltage and resistance measurement:");
  Serial.println("\n   Vo    Rt");
}


void loop() {
  // put your main code here, to run repeatedly:
  char input;
  uint8_t i;

  // Push 3 values into the Average buffer.  Any old values will drop off the bottom
  for (int i = 0; i < 3; i++){
    x = i;
    Vo = int(analogRead(ThermistorPin));
    Rt = R*(1023.0/float(Vo)-1.0);
    voltage[x] = Vo;
    delay(1000);
  }

  Serial.print("Average voltage is: ");
  Serial.println((voltage[0]+voltage[1]+voltage[2])/3);
  
  
  if(Serial.available() > 0){
    input = Serial.read(); //user input for increase or decrease
    if(input == ' '){
      power += 17;
      change = 1;
    }else if(input == 'r'){
      power -= 17;
      change = 1;
    }

    if ( power > 255 ){
      Serial.println("output level is too high, output set to 255 (max)");
      power = 255;
    }else if( power < 0 ){
      Serial.println("output level is too low, output set to 0 (min)");
      power = 0;
    }

    if (change == 1){
      analogWrite(PeltierPin, power); //Write this new value out to the port
      Serial.print("output is now changed to: ");
      Serial.println(power);
      change = 0;
    }
delay(250);
  }
}
