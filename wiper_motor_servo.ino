//Wiper motor software v 1.3:
//Servo sensor to Analog 0
//Setpoint potentiometer to Analog 1
//H bridge direction pin to Digital 2
//H bridge PWM signal to Digital 3
//LEDs at digital pins 4 - 13 are optional (indicating setpoint as LED bar)
//
//For the first test run, set MAX_DUTYCYCLE to 75 in order to lower the maximum torque in case something goes wrong.
//When connecting the servo to the microcontroller for the first time, you have to consider the rotational direction of your wiper motor!
//Adjust the potentiometer defining the setpoint to the center position.
//If the servo moves to neutral position, too after connecting the circuit to the supply voltage, the polarity of the servo sensor is correct.
//If the wiper motor starts spinning away from the center position, the polarity of the servo sensor (+ and - connector) has to be swapped.
//
//
//Source & info: www.HomoFaciens.de/technics-computer-arduino-uno_en_navion.htm


//#include <avr/io.h>
//#include <util/delay.h>


int ADC_SetPoint = 0;
int ADC_SetPointOld = 0;
int ADC_ServoPoti = 0;
int ADC_ServoPotiOld = 0;
int dutyCycle = 50; // 10 - 255
int ADCdiff = 0;
int timeDiff = 0;

//Change values below to adapt your motor
//Set MAX_DUTYCYCLE to 75 for the first test run!

#define P_FRACTION 0.3         //0.0 - 10.0 (0.3)
#define I_FRACTION 0.3         //0.0 - 10.0 (0.3)
#define D_FRACTION 4.0         //0.0 - 10.0 (4.0)
#define V_WINDOW 25            //10 - 1000 (25)
#define MIN_DUTYCYCLE 25       //0 - 255 (25)
#define MAX_DUTYCYCLE 200      //0 - 255 (255)
#define SOFT_START 0.3        //0.00 - 1.00 (0.30) 1.00 = OFF
#define D_FRACTION_DEMO 0      //1 - consider only D_FRACTION for servo movement (0 - OFF)
#define EMERGENCY_SHUTDOWN 4   //0 - 100 (4), 0 - OFF, Stops motor if blocked
#define SHOOT_THROUGH_PAUSE 10 //Prevent H bridge from shoot through whenever the direction pin is changed

#define SERVO_DIRECTION_PIN 2 //Direction pin servo motor
#define SERVO_PWM_PIN 3       //PWM pin servo motor
#define SERVO_SENSOR A0       //Analog input servo sensor
#define POTI A1               //Analog input potentiometer
#define LED_01 4
#define LED_02 5
#define LED_03 6
#define LED_04 7
#define LED_05 8
#define LED_06 9
#define LED_07 10
#define LED_08 11
#define LED_09 12
#define LED_10 13


void setup(){
  pinMode(SERVO_DIRECTION_PIN, OUTPUT);     
  pinMode(SERVO_PWM_PIN, OUTPUT);     

  pinMode(SERVO_SENSOR, INPUT);     
  pinMode(POTI, INPUT);     

  //LED bar (optional)
  pinMode(LED_01, OUTPUT);     
  pinMode(LED_02, OUTPUT);     
  pinMode(LED_03, OUTPUT);     
  pinMode(LED_04, OUTPUT);     
  pinMode(LED_05, OUTPUT);     
  pinMode(LED_06, OUTPUT);     
  pinMode(LED_07, OUTPUT);     
  pinMode(LED_08, OUTPUT);     
  pinMode(LED_09, OUTPUT);     
  pinMode(LED_10, OUTPUT);     
  
} 

 
void loop(){ 
  ADC_ServoPoti = analogRead(0);     // reads the servo sensor (between 0 and 1023) 
  ADC_SetPoint = analogRead(1);             
    
  ADCdiff = ADC_SetPoint - ADC_ServoPoti;
  
  dutyCycle = abs(ADCdiff) * P_FRACTION;
  dutyCycle += timeDiff * I_FRACTION;
  dutyCycle += abs(ADC_SetPointOld - ADC_SetPoint) * D_FRACTION;
  
  if(D_FRACTION_DEMO == 1){
    dutyCycle = abs(ADC_SetPointOld - ADC_SetPoint) * D_FRACTION;
  }
  
  if(SOFT_START * timeDiff < 1){
    dutyCycle = dutyCycle * (SOFT_START * timeDiff);
  }
  
  timeDiff++;
  
  if(dutyCycle < MIN_DUTYCYCLE && dutyCycle > 0){
    dutyCycle = MIN_DUTYCYCLE;
  }
  
  if(dutyCycle > MAX_DUTYCYCLE){
    dutyCycle = MAX_DUTYCYCLE;
  }
  
  if(dutyCycle < 0){
    dutyCycle = 0;
  }
  
  
  
  if(D_FRACTION_DEMO == 1){
    if(abs(ADC_SetPointOld - ADC_SetPoint) < 2){
      analogWrite(SERVO_PWM_PIN, 0);
      delayMicroseconds(SHOOT_THROUGH_PAUSE);
      digitalWrite(SERVO_DIRECTION_PIN, 0);
      delayMicroseconds(SHOOT_THROUGH_PAUSE);
    }
    else{
      if(ADC_SetPointOld - ADC_SetPoint < 0){
        analogWrite(SERVO_PWM_PIN, 0);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        digitalWrite(SERVO_DIRECTION_PIN, 0);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        analogWrite(SERVO_PWM_PIN, dutyCycle);
      }
      if(ADC_SetPointOld - ADC_SetPoint > 0){
        analogWrite(SERVO_PWM_PIN, 0);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        digitalWrite(SERVO_DIRECTION_PIN, 1);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        analogWrite(SERVO_PWM_PIN, dutyCycle);
      }
    }
  }
  else{
    if(abs(ADCdiff) < V_WINDOW){
      dutyCycle = 0;
      timeDiff = 0;
    }

    if(abs(ADC_ServoPotiOld - ADC_ServoPoti) < EMERGENCY_SHUTDOWN && dutyCycle == MAX_DUTYCYCLE && timeDiff > 50){
      analogWrite(SERVO_PWM_PIN, 0);
      delayMicroseconds(SHOOT_THROUGH_PAUSE);
      digitalWrite(SERVO_DIRECTION_PIN, 0);
      delay(1000);
      timeDiff = 0;
    }
    else{  
      if(ADCdiff > 0){
        analogWrite(SERVO_PWM_PIN, 0);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        digitalWrite(SERVO_DIRECTION_PIN, 0);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        analogWrite(SERVO_PWM_PIN, dutyCycle);
      }
      if(ADCdiff < 0){
        analogWrite(SERVO_PWM_PIN, 0);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        digitalWrite(SERVO_DIRECTION_PIN, 1);
        delayMicroseconds(SHOOT_THROUGH_PAUSE);
        analogWrite(SERVO_PWM_PIN, dutyCycle);
      }
    }
  }
  

  ADC_SetPointOld = ADC_SetPoint;
  ADC_ServoPotiOld = ADC_ServoPoti;


  //LED bar at digital pins 4 - 13 (optional)
  
  if(ADC_SetPoint >= 93 * 10){
    digitalWrite(LED_10, 1);
  }
  else{
    digitalWrite(LED_10, 0);
  }

  if(ADC_SetPoint >= 93 * 9){
    digitalWrite(LED_09, 1);
  }
  else{
    digitalWrite(LED_09, 0);
  }

  if(ADC_SetPoint >= 93 * 8){
    digitalWrite(LED_08, 1);
  }
  else{
    digitalWrite(LED_08, 0);
  }

  if(ADC_SetPoint >= 93 * 7){
    digitalWrite(LED_07, 1);
  }
  else{
    digitalWrite(LED_07, 0);
  }

  if(ADC_SetPoint >= 93 * 6){
    digitalWrite(LED_06, 1);
  }
  else{
    digitalWrite(LED_06, 0);
  }

  if(ADC_SetPoint >= 93 * 5){
    digitalWrite(LED_05, 1);
  }
  else{
    digitalWrite(LED_05, 0);
  }

  if(ADC_SetPoint >= 93 * 4){
    digitalWrite(LED_04, 1);
  }
  else{
    digitalWrite(LED_04, 0);
  }

  if(ADC_SetPoint >= 93 * 3){
    digitalWrite(LED_03, 1);
  }
  else{
    digitalWrite(LED_03, 0);
  }

  if(ADC_SetPoint >= 93 * 2){
    digitalWrite(LED_02, 1);
  }
  else{
    digitalWrite(LED_02, 0);
  }

  if(ADC_SetPoint >= 93 * 1){
    digitalWrite(LED_01, 1);
  }
  else{
    digitalWrite(LED_01, 0);
  }


  delay(15);                             // waits for the servo to get there 

}



