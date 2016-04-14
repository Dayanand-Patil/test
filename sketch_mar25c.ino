#include <Servo.h>   // Use the servo control library
#include <LiquidCrystal.h>

//Macros


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo motor;         // Create a variable to hold servo motor controller

//Constants:
const int potPin = A0; //pin A0 to read analog input

int button = 6;

//Variables:
int value = 0; //save analog value
int angle = 0;

int dir = 0;

void setup()
{
 
  pinMode(potPin, INPUT); 
  pinMode(button, INPUT); 
  
  motor.attach(13);  // Connect the controller to the servo on pin 13

  Serial.begin(9600);
  //Serial.print("\n Analog Value is:- ");

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);  
}

void loop()
{ 
  //delay(2000);
  
  //delay(10);

  if(digitalRead(button) == HIGH)
  {
    dir = dir + 1;    
    //Serial.print("\n Button Pressed! ");    
  }

  if(dir >= 2)
  {
    dir = 0;
  }
    
  value = analogRead(potPin);          //Read and save analog value from potentiometer

  Serial.print("\n\n\n");
  Serial.print("\n ------START--->>>>>>>");
  Serial.print("\n Analog Value:- ");
  Serial.print(value);

  
  // Print a message to the LCD.     
  
  angle = value / 5.7;    

  if(dir == 1)
  {
    angle = 180 - angle;
    Serial.print("\n Button Pressed! ");
    Serial.print("\n Direction is opposite to that Potentiometer!");
  }
    
  Serial.print("\n Motor Angle:- ");
  Serial.print(angle);
  Serial.print("\n xxxxxxxx END xxxxxxx");

  Serial.print("\n Direction is same to that Potentiometer!");
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("AnalogValue:");
  lcd.print(value); 
  lcd.setCursor(0, 1);
  lcd.print("Angle Value:");
  lcd.print(angle);  

  delay(1000);
  motor.write(angle); 

  //lcd.clear();
  //delay(500);   
}


