#define dirPinL 8
#define dirPinR 2
#define pwrPinL 9
#define pwrPinR 3
#define ch1pin 4
#define ch2pin 5
#define statuslight 13

unsigned long ch1;
unsigned long ch2;

int power;
int steer;

void setup() {
  //increase PWM frequency!
  TCCR1B = 0x01;
  pinMode(dirPinL, OUTPUT);
  pinMode(dirPinR, OUTPUT);
  analogWrite(pwrPinL, 0);
  analogWrite(pwrPinR, 0);
  pinMode(ch1pin, INPUT_PULLUP);
  pinMode(ch2pin, INPUT_PULLUP);
  pinMode(statuslight, OUTPUT);
  for(int i = 0; i < 10; i++){
    digitalWrite(statuslight, HIGH);
    delay(25);
    digitalWrite(statuslight,LOW);
    delay(50);
  }
  Serial.begin(115200);
  setMotor(100, 0);
  setMotor(100, 1);
  delay(1000);
  setMotor(-100, 0);
  setMotor(-100, 1);
}

void loop() {
  getTxValues(&ch1, &ch2);
  power = (1500-ch2)/2;
  steer = (((ch1-1500)/2));
  setMotor((int)(power-steer),0);
  setMotor((int)(power+steer),1);
}

void setMotor(int power, char motor) {
  if(power > 255) {
    power = 255;
  } else if(power < -255){
    power = -255;
  } else if(abs(power) < 15) {
    power = 0;
  }

  Serial.println(power);
  if(motor == 0){   
    if(power > 0) {
      digitalWrite(dirPinL, LOW);
      analogWrite(pwrPinL, power);
    } else if(power < 0) {
      digitalWrite(dirPinL, HIGH);
      analogWrite(pwrPinL, 255-power);
    } else {
      digitalWrite(dirPinL, LOW);
      analogWrite(pwrPinL, 0);
    }
  } else if(motor == 1){  
    if(power > 0) {
      digitalWrite(dirPinR, LOW);
      analogWrite(pwrPinR, power);
    } else if(power < 0) {
      digitalWrite(dirPinR, HIGH);
      analogWrite(pwrPinR, 255-power);
    } else {
      digitalWrite(dirPinR, LOW);
      analogWrite(pwrPinR, 0);
    }
  }
    
}

void getTxValues(unsigned long* ch1, unsigned long* ch2){
    *ch1 = pulseIn(ch1pin, HIGH);
    *ch2 = pulseIn(ch2pin, HIGH);
}
  
