#include <OneWire.h> 
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 10 on the Arduino 
#define ONE_WIRE_BUS 10 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
double aircon = 27.0;

void setup() 
{
  Serial.begin(9600);
  //pinMode(SIGNAL_PIN, INPUT);
  //digitalWrite(SIGNAL_PIN, LOW);
  sensors.begin();
  pinMode(8, INPUT);
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.print("Aircon temperature is ");
  Serial.println(aircon);
}

unsigned long time_now = 0;
bool master = false;
bool human = false;
int lastButtonState = 0;
int buttonState = 0;
int airconSwitch = 0;
bool gotPeople = false;
void loop() 
{
  
  if(Serial.available()) {  //id data is available to read
    char num = Serial.read();

    if(num == 'o'){           //if o received
      Serial.println("on");
      //master = true;
      airconSwitch = HIGH;
      gotPeople = false; //Step in before 1hour.
      human = false; //if people exist in class.
      time_now = millis();
      Serial.println("Aircon is on");
      digitalWrite(6, airconSwitch); //turn on red led
    }
    if(num == 'f'){           //if f received
      Serial.println("off");
      airconSwitch = LOW;
      gotPeople = false;
      human = false;
      Serial.println("Aircon is off");
      digitalWrite(4, LOW);
      digitalWrite(6, airconSwitch);  //turn off red led
    }
  }
  buttonState = digitalRead(8);
  if((millis() > time_now + 10000) and !gotPeople){
    // Switch off aircon after inactivity
    digitalWrite(6, LOW);
    time_now = millis();
  }
  
  if(buttonState == HIGH){
    human = true;
    gotPeople = true;
  } else {
    human = false;
  }
  
  if(millis() > time_now + 2000){
    if (master || human){
      Serial.println("Movement detected.");
      digitalWrite(4, HIGH);
      Serial.print("Requesting temperatures..."); 
      sensors.requestTemperatures(); // Send the command to get temperature readings 
      Serial.println("DONE"); 
      /********************************************************************/
      Serial.print("Temperature is: "); 
      double temperature = sensors.getTempCByIndex(0);
      Serial.println(temperature); 
      if(temperature <= 24.0 and aircon <= 24.0) {
        Serial.print("Room temperature too low. Aircon setting too low. Adjusting aircon to: ");
        aircon += 1.0;
      } 
      if(temperature <= 24.0 and aircon >= 26.0) {
        Serial.print("Room temperature too low. Aircon setting too high. Adjusting aircon to: ");
        aircon -= 1.0;
        Serial.println(aircon);
      }
      if(temperature >= 23.0 and aircon <= 21.0) {
        Serial.print("Room temperature too high. Aircon setting too low. Adjusting aircon to: ");
        aircon += 1.0;
        Serial.println(aircon);
      }
      if(temperature >= 23.0 and aircon >= 23.0){
        Serial.print("Temperature too high. Aircon setting too high. Adjusting aircon to: ");
        aircon -= 1.0;
        Serial.println(aircon);
      }
      Serial.print("Aircon temperature is: ");
      Serial.println(aircon);
    }else if(gotPeople){
      Serial.println("Did not any detect movement.");
      digitalWrite(4, LOW);
      digitalWrite(6, LOW);
    }
  }
  
  
}
