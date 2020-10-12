//1.8 degrees (200 steps per revolution)
// library defines speed as steps per second

#define debug
//#define testMotor

#include <AccelStepper.h>
// motor connections:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1 // must be set to 1 when using a driver
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

#define swDir_p A0 //INPUT_PULLUP
#define swDir_p1 A1 //INPUT_PULLUP
#define accel_pot_p A2 //INPUT_PULLUP
#define spd_pot_p A3 //INPUT_PULLUP

#define spd 1000

bool bDir=true;

void setup() {
  pinMode(swDir_p, INPUT_PULLUP);
  pinMode(swDir_p1, INPUT_PULLUP);
  pinMode(accel_pot_p, INPUT_PULLUP);
  pinMode(spd_pot_p, INPUT_PULLUP);

  stepper.setMaxSpeed(800); //1000 is max for this lib, also 1000 is peak of noise for some motors
  stepper.setAcceleration(2000);

/*
  if(analogRead(accel_pot_p)<900)
  {
    stepper.setAcceleration(max(map(analogRead(accel_pot_p),0,900,100,1000),1000));
  }
  if(analogRead(spd_pot_p)<900)
  {
    stepper.setMaxSpeed(max(map(analogRead(spd_pot_p),0,900,100,1000),1000));
  }


  while(!digitalRead(swDir_p) && !digitalRead(swDir_p1)) //both detectors on, wait fixed
  {
	   digitalWrite(LED_BUILTIN, HIGH); delay(300);  
	   digitalWrite(LED_BUILTIN, LOW); delay(300); 
  }
  if(!digitalRead(swDir_p) || !digitalRead(swDir_p1))
  {
	 digitalWrite(LED_BUILTIN, HIGH); delay(100);  
	 digitalWrite(LED_BUILTIN, LOW); delay(100); 
	 digitalWrite(LED_BUILTIN, HIGH); delay(100);  
	 digitalWrite(LED_BUILTIN, LOW); delay(100); 
	 digitalWrite(LED_BUILTIN, HIGH); delay(100);  
	 digitalWrite(LED_BUILTIN, LOW); delay(100); 
  }
*/
    stepper.moveTo(6000000);
  #ifdef debug
  Serial.begin(57600);
  #endif
}

long sw_last_t=0;
long nextWait_t=10000;
#ifdef debug
long nextPrintDebug_t=0;
#endif
void loop() {
	#ifdef testMotor
  	if(millis()/10000%2) stepper.moveTo(60000);
	else  stepper.moveTo(-60000);
	stepper.run() ;
	return;
	#endif
	
	
  if(!digitalRead(swDir_p) && !digitalRead(swDir_p1) || millis()>sw_last_t+60000 )
  {
    stepper.stop();
    delay(100);

    bDir=!bDir;
    //stepper.setCurrentPosition(0);
    stepper.moveTo(bDir?6000000:-6000000);

    sw_last_t=millis();
    
    return;
  }
  if(!digitalRead(swDir_p))
  {
    //stepper.setCurrentPosition(0);
    
    bDir=true;
    //stepper.setMaxSpeed(spd);
    
    //stepper.setSpeed(spd);
    stepper.moveTo(6000000);

    sw_last_t=millis();

    
  }
  if(!digitalRead(swDir_p1))
  {
    //stepper.setCurrentPosition(0);
    
    bDir=false;
    //stepper.setMaxSpeed(-spd);

    //stepper.setSpeed(-spd);
    stepper.moveTo(-6000000);

    sw_last_t=millis();
  }


  
  stepper.run() ;

  if(abs(stepper.speed())<100 && millis()>nextWait_t)  ///100%10==0 
  {
	 #ifdef debug
     Serial.println(millis());
	 #endif
    
     nextWait_t=millis()+60000;
     stepper.stop();
     stepper.setCurrentPosition(0);

     delay(2000);

     stepper.moveTo(bDir?6000000:-6000000);
     
	 #ifdef debug
     Serial.println(millis());
	 #endif
  }
	
  
  #ifdef debug
  if(millis()>nextPrintDebug_t)
  {
	  nextPrintDebug_t=millis()+20;
	  Serial.print(digitalRead(swDir_p));Serial.print("\t");
	  Serial.print(analogRead(swDir_p));Serial.print("\t");
	  
	  Serial.print(digitalRead(swDir_p1));Serial.print("\t");
	  Serial.print(analogRead(swDir_p1));Serial.print("\t");
	  
	  Serial.println(stepper.speed());
  }	
  #endif
  
  // Set the target position:
  //stepper.moveTo(8000);
  // Run to target position with set speed and acceleration/deceleration:
  //stepper.runToPosition();
  //delay(1000);
  // Move back to zero:
  // stepper.moveTo(0);
  //stepper.runToPosition();
  //delay(1000);
}
