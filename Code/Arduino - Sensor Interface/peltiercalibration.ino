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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //starts the serial monitor
  pinMode(PeltierPin, OUTPUT); //sets the defined PeltierPin to output mode (to send voltage to Peltier)
  Serial.println("Press the space bar, then hit enter, to move to the next output");
  Serial.println("Press the r key, then hit enter, to go back to the previous output");
}


void loop() {
  // put your main code here, to run repeatedly:
  char input;
  uint8_t i;
  float average;
  samples = 0;

  // take voltage readings from the voltage divider
  digitalWrite(vd_power_pin, HIGH);
  for (i = 0; i < samplingrate; i++) {
    samples += analogRead(ntc_pin);
    delay(100);

  }
  digitalWrite(vd_power_pin, HIGH);
  average = 0;
  average = samples / samplingrate;
  Serial.println("\n \n");
  Serial.print("ADC readings ");
  Serial.println(average);
  
  // Calculate NTC resistance
  average = 1023 / average - 1;
  average = Rref / average;
  Serial.print("Thermistor resistance ");
  Serial.println(average);
delay(2000);
  
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
