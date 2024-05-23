
//this sketch uses a P controller to control two DG0D1-E hobby motors
// the P controller is set up to  maintain a target speed for each motor
// https://cdn.sparkfun.com/assets/8/3/b/e/4/DS-16413-DG01D-E_Motor_with_Encoder.pdf for motor documentation

#include <util/atomic.h>

//define pins
#define ENCRA 20 //falls on forward falls on revers / h2
#define ENCRB 21 //rises on forward falls on reverse / h1
#define ENCLA 18
#define ENCLB 19
#define Rwb 4
#define Rwf 5
#define Lwf 7
#define Lwb 6

//variables for the P controller
long prevT = 0;
int posRPrev = 0;
volatile int posR_i = 0;
float eintR = 0;
int posLPrev = 0;
volatile int posL_i = 0;
float eintL = 0;

//variables for filtering
float vRFilt=0;
float vRPrev=0;
float vLFilt=0;
float vLPrev=0;



void setup() {
  //serial monitor
    Serial.begin(9600);

  //set pins
  pinMode(ENCRA, INPUT);
  pinMode(ENCRB, INPUT);
  pinMode(Rwb, OUTPUT);
  pinMode(Rwf, OUTPUT);
  pinMode(Lwb, OUTPUT);
  pinMode(Lwf, OUTPUT);  
  pinMode(13, OUTPUT);

 //attach interrupts
 attachInterrupt(digitalPinToInterrupt(ENCRB), readEncoderR, RISING); 
 attachInterrupt(digitalPinToInterrupt(ENCLB), readEncoderL, FALLING); 

}

void loop() {

  //position vars for each encoder in each iteration
  int posR = 0;
  int posL = 0;


 //update distance in an atomic block to prevent interrupts
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    posR = posR_i;
    posL = posL_i;
  }

  //update time
  long currT = micros();
  float deltaT = ((currT-prevT)/(1.0e6));

  //calc velocities and record position for next iteration
  float vR = (posR - posRPrev)/deltaT;
  posRPrev = posR;
  float vL = (posL - posLPrev)/deltaT;
  posLPrev = posL;
  prevT = currT;

  //filter velocities and record for the next iteration
  vRFilt = 0.854*vRFilt + 0.0728*vR + 0.0728*vRPrev;
  vRPrev = vR;
  vLFilt = 0.854*vLFilt + 0.0728*vL + 0.0728*vLPrev;
  vLPrev = vL;

  //target "speed"
  float vtarget =  145;

  //control signal calculations. in this PID there are no ki and kd terms but they can be easily added
  //check out https://www.youtube.com/watch?v=HRaZLCBFVDE&ab_channel=CurioRes for how to do this
  //kp values can be tuned at students discretion, I do not recommend exceeding 5.5 though each motor is unique and should be tuned individually
  float kpR = 4.75;
  float kpL = 5.25;
//  float kiR = 0;
//  float kiL = 0;
  
  float eR = vtarget - vRFilt;
//  eintR = eintR + eR*deltaT;
  float uR = kpR*eR;
  float eL = vtarget - vLFilt;
//  eintL = eintL + eL*deltaT;
  float uL = kpL*eL;

  //make sure control signal is within output bounds
  if(uR>255){
    uR = 255;
  }

  if(uL>255){
    uL = 255;
  }

  //send control signals to motors
  analogWrite(Rwf, uR);
  analogWrite(Lwf, uL);
  
  //serial monitor stuff
  Serial.print(uR);
  Serial.print (" ");
  Serial.print (145);
  Serial.print (" ");
  Serial.print(uL);
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

void readEncoderL(){
   int a = digitalRead(ENCLA);
   if(a>0){
    posL_i--;
   }
   else{
    posL_i++;
   }
}
