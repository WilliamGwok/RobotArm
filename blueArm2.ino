#include<Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial BTserial(2,3);

Servo base,fArm,rArm,claw;

int basePos=90;
int fArmPos=90;
int rArmPos=90;
int clawPos=90;

const int basemin=0;
const int basemax=180;
const int fArmmin=45;
const int fArmmax=90;
const int rArmmin=30;
const int rArmmax=180;
const int clawmin=0;
const int clawmax=90;

int DSD=15;//控制运行速度；

bool mode;//控制工作模式；1 指令 0 手柄；
int moveStep=3;//手柄移动量；

void setup() {
  BTserial.begin(9600);
  BTserial.println("AT");
  

  base.attach(11);
  delay(200);
  rArm.attach(10);
  delay(200);
  fArm.attach(9);
  delay(200);
  claw.attach(6);
  delay(200);
  Serial.begin(9600);
  Serial.println("Hello!This is Will's MeArm!");
}

void loop() {
  if(BTserial.available()>0){
    char serialCmd=BTserial.read();
    if(mode==1){
      armDataCmd(serialCmd);//指令模式
    }else{
      armJoyCmd(serialCmd);//手柄模式
    } 
  }
  
  if(Serial.available()>0){
    char serialCmd=Serial.read();
    if(mode==1){
      armDataCmd(serialCmd);//指令模式
    }else{
      armJoyCmd(serialCmd);//手柄模式
    } 
  }
  
//  base.write(basePos);
//  delay(10);
//  fArm.write(fArmPos);
//  delay(10);
//  rArm.write(rArmPos);
//  delay(10);
//  claw.write(clawPos);
//  delay(10);
}

void armDataCmd(char serialCmd){

  if(serialCmd=='w' || serialCmd=='s' || serialCmd=='a' || serialCmd=='d'
     || serialCmd=='j' || serialCmd=='u' || serialCmd=='k' || serialCmd=='l'){
      Serial.println("+Warning: Robot in Instruction Mode...");
      delay(100);
      while(Serial.available()>0) char wrongCommand =Serial.read();
      return;
     }


  if(serialCmd=='b' || serialCmd=='c' || serialCmd=='f' || serialCmd=='r'){
    int servoData=Serial.parseInt();
    servoCmd(serialCmd,servoData,DSD);
  }else{
    switch(serialCmd){
      case 'm':
        mode=0;
        Serial.println("Command: Switch to Joy-Stick Mode.");
        break;
      case 'o':
        reportStatus();
        break;
      case 'i':
        armIniPos();
        break;
      default:
        Serial.println("Unknown Command");
    }
  }
}

void servoCmd(char servoName,int toPos,int servoDelay){
  Servo servo2go;

  Serial.println("");
  Serial.print("+Command:Servo ");
  Serial.print(servoName);
  Serial.print(" to ");
  Serial.print(toPos);
  Serial.print(" at servoDelay value ");
  Serial.print(servoDelay);
  Serial.println(".");
  Serial.println("");

  int fromPos;

    switch(servoName){
      case 'b':
        if(toPos >= basemin && toPos <= basemax){
          servo2go=base;
          fromPos=base.read();
           break;
        }else{
          Serial.println("+Warning: Base Servo Value Out Of Limit!");
          return;
        }
        
      case 'c':
        if(toPos >= clawmin && toPos <= clawmax){
          servo2go=claw;
          fromPos=claw.read();
           break;
        }else{
          Serial.println("+Warning: Claw Servo Value Out Of Limit!");
          return;
        }
        
      case 'f':
        if(toPos >= fArmmin && toPos <= fArmmax){
          servo2go=fArm;
          fromPos=fArm.read();
           break;
        }else{
          Serial.println("+Warning: fArm Servo Value Out Of Limit!");
          return;
        }

      case 'r':
        if(toPos >= rArmmin && toPos <= rArmmax){
          servo2go=rArm;
          fromPos=rArm.read();
           break;
        }else{
          Serial.println("+Warning: rArm Servo Value Out Of Limit!");
          return;
        }
    }//赋值给servo2go；

    if(fromPos<=toPos){
      for(int i=fromPos;i<=toPos;i++){
        servo2go.write(i);
        delay(servoDelay);
      }
    }else{
          for(int i=fromPos;i>=toPos;i--){
           servo2go.write(i);
           delay(servoDelay);
        }
     }
}

void armJoyCmd(char serialCmd){
  if(serialCmd=='r' || serialCmd=='f' || serialCmd=='c' || serialCmd=='b'){
      Serial.println("+Warning: Robot in Joy-Stick Mode...");
      delay(100);
      while(Serial.available()>0) char wrongCommand =Serial.read();
      return;
     }
  int baseJoyPos;
  int rArmJoyPos;
  int fArmJoyPos;
  int clawJoyPos;
  switch(serialCmd){
    case 'a'://base
      Serial.println("Receive Command: Base Turn left");
      baseJoyPos=base.read()+moveStep;
      servoCmd('b',baseJoyPos,DSD);
      break;

    case 'd':
      Serial.println("Receive Command: Base Turn Right");
      baseJoyPos=base.read()-moveStep;
      servoCmd('b',baseJoyPos,DSD);
      break;

    case 's'://rArm
      Serial.println("Receive Command: rArm Down");
      rArmJoyPos=rArm.read()-moveStep;
      servoCmd('r',rArmJoyPos,DSD);
      break;

    case 'w'://rArm
      Serial.println("Receive Command: rArm Up");
      rArmJoyPos=rArm.read()+moveStep;
      servoCmd('r',rArmJoyPos,DSD);
      break;

    case 'k'://fArm
      Serial.println("Receive Command: fArm Down");
      fArmJoyPos=fArm.read()-moveStep;
      servoCmd('f',fArmJoyPos,DSD);
      break;

    case 'u'://fArm
      Serial.println("Receive Command: fArm Up");
      fArmJoyPos=fArm.read()+moveStep;
      servoCmd('f',fArmJoyPos,DSD);
      break;

    case 'l'://claw
      Serial.println("Receive Command: Claw Open Up");
      clawJoyPos=claw.read()-moveStep;
      servoCmd('c',clawJoyPos,DSD);
      break;

    case 'j'://claw
      Serial.println("Receive Command: Claw Close Down");
      clawJoyPos=claw.read()+moveStep;
      servoCmd('c',clawJoyPos,DSD);
      break;

    case 'o':
      reportStatus();
      break;

    case 'i':
      armIniPos();
      break;

    case 'm':
      mode=1;
      Serial.println("Command: Switch to Instruction Mode.");
      break;

    default:
      Serial.println("Unknown Command...");
      return;
  }
}

void reportStatus(){
  Serial.println("");
  Serial.println("");
  Serial.println("+ Robot-Arm Status Report +");
  Serial.print("Claw Position: ");Serial.println(claw.read());
  Serial.print("Base Position: ");Serial.println(base.read());
  Serial.print("fArm Position: ");Serial.println(fArm.read());
  Serial.print("rArm Position: ");Serial.println(rArm.read());
  Serial.println("++++++++++++++++++++++++++");
  Serial.println("");
}

void armIniPos(){
  Serial.println("+Command: Restore Initial Position.");
  int robotIniPosArray[4][3]={
    {'b',90,DSD},
    {'r',90,DSD},
    {'f',90,DSD},
    {'c',90,DSD}
  };
  for(int i=0;i<4;i++){
    servoCmd(robotIniPosArray[i][0],robotIniPosArray[i][1],robotIniPosArray[i][2]);
  }
}
