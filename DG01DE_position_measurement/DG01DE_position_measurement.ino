// this sketch reads position from a DG01DE hobby motor with encoder
// https://cdn.sparkfun.com/assets/8/3/b/e/4/DS-16413-DG01D-E_Motor_with_Encoder.pdf


//define pins
#define ENCA 18 //falls on forward falls on revers / h2
#define ENCB 19 //rises on forward falls on reverse / h1
#define Rwb 6
#define Rwf 7
#define light 14


//position var
int pos = 0;

void setup() {
  //connect to serial monitor
    Serial.begin(9600);

 //set pins
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(Rwb, OUTPUT);
  pinMode(Rwf, OUTPUT);

//attach interrupt
 attachInterrupt(digitalPinToInterrupt(ENCB), readEncoder, RISING); 
 

}

void loop() {
  // put your main code here, to run repeatedly:
  //analogWrite(Rwf, 150);
  //int b = digitalRead(ENCB);
  //int a = digitalRead(ENCA);
  //Serial.print(a*5);
  //Serial.print(" ");
  //Serial.print(b*5);
  //Serial.println();

  Serial.println(pos);


}

//this interrupt updates the wheels position. depending the orientation of the motor you will may need to swap the arithmetic
void readEncoder(){
   int a = digitalRead(ENCA);
   if(a>0){
    pos++; //left whell ++ right wheel --
   }
   else{
    pos--; //left wheel -- right wheel ++
   }
}
