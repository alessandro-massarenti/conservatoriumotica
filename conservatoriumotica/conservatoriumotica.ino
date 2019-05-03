int l = 0;  //Current level of the tank
int M = 80; //Maximum level of the tank
int m = 20; //Minimum level of the tank

int discharge = ; //Discharge valve control pin
int charge = ; //Charge valve control pin

void setup() {
  // put your setup code here, to run once:
   pinMode(discharge, OUTPUT);
   pinMode(charge,OUTPUT);
}

void loop() {
  if(l>M){
    digitalWrite(discharge,HIGH);
  }else if(l<m){
    digitalWrite(charge,HIGH);
  }else{
    digitalWrite(discharge,HIGH);
    digitalWrite(charge,HIGH);
  }
}
