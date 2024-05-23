// this sketch reads values from the encoders from a DG01DE hobby motor with encoder
// it plots each encoder value to the serial plotter and should be used to determine whether an interrupt should be set to RISING or FALLING
// and which encoder should be used to trigger the interrupt and which should be read in the interrupt
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

 

}

void loop() {
  // read encoders and print to serial plotter
  int b = digitalRead(ENCB);
  int a = digitalRead(ENCA);
  Serial.print(a*5);
  Serial.print(" ");
  Serial.print(b*5);
  Serial.println();



}
