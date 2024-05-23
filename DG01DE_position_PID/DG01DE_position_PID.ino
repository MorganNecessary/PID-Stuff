
//this sketch uses a P controller to control two DG0D1-E hobby motors
// the P controller is set up to  maintain a target speed for each motor
// https://cdn.sparkfun.com/assets/8/3/b/e/4/DS-16413-DG01D-E_Motor_with_Encoder.pdf for motor documentation

#include <util/atomic.h>

//define pins
#define ENCRA 20 //falls on forward falls on revers / h2
#define ENCRB 21 //rises on forward falls on reverse / h1
#define Rwb 4
#define Rwf 5

//variables for the P controller
int posRPrev = 0;
volatile int posR_i = 0;



void setup() {
  //serial monitor
    Serial.begin(9600);

  //set pins
  pinMode(ENCRA, INPUT);
  pinMode(ENCRB, INPUT);
  pinMode(Rwb, OUTPUT);
  pinMode(Rwf, OUTPUT);

 //attach interrupts
 attachInterrupt(digitalPinToInterrupt(ENCRB), readEncoderR, RISING);  

}

void loop() {

  //position var in each iteration
  int posR = 0;


 //update distance in an atomic block to prevent interrupts
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    posR = posR_i;
  }


  //target position
  float ptarget =  150;

  //control signal calculations. in this PID there are no ki and kd terms but they can be easily added
  //check out https://www.youtube.com/watch?v=HRaZLCBFVDE&ab_channel=CurioRes for how to do this
  //kp values can be tuned at students discretion, I do not recommend exceeding 5.5 though each motor is unique and should be tuned individually
  float kpR = 5;
  float eR = ptarget - posR;
  float uR = kpR*eR;

 
  if(uR > 0){ //uR positive is go forwards
     //make sure control signal is within output bounds
    if(uR>255){
      uR = 255;
    }
    analogWrite(Rwf, uR);  //send control signals to motors
  }
  else if(uR < 0){ //uR negative is go backwards
     //make sure control signal is within output bounds
    if(uR<255){
      uR = 255;
    }
    analogWrite(Rwb, uR);  //send control signals to motors
  }
  else{
    //do nothing
  }
 
  
  //serial monitor stuff
  Serial.print(uR);
  Serial.print (" ");
  Serial.print (150);
  Serial.println();
  delay(1);
}

//these interrupts read the encoder values for each motor and update positions.
void readEncoderR(){
   int a = digitalRead(ENCRA);
   if(a>0){
    posR_i--;
   }
   else{
    posR_i++;
   }
}
