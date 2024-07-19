//pin define
#define ledbtn 2//          led 레드, 화이트 변경 버튼핀
#define trigPin 3//         초음파 트리거핀
#define echoPin 4//         초음파 에코핀
#define ftrigPin 5//        초음파 트리거핀
#define fechoPin 6//        초음파 에코핀
#define strigPin 7//        초음파 트리거핀
#define sechoPin 8//        초음파 에코핀
#define ttrigPin 9//        초음파 트리거핀
#define techoPin 10//       초음파 에코핀
#define redlight_out 11//   빨간 불 출력핀
#define whitelight_out 12// 하얀 불 출력핀
#define buzzer 13//         피에조 부저
#define selector1 0//셀렉터 핀
#define selector2 1//셀렉터 핀
#define selector3 2//셀렉터 핀
//-------------    변수    --------------
int start = 0;
int senseerror = 0;//초음파 센싱이 몇번 안될때 사용
int lowdist = 300;//초음파 동작 거리 최소
int middist = 450;//중간거리
int highdist = 600;//초음파 동작 거리 최대
#define deltime 500//깜빡임 시간
long distancez = 3000, distancef = 3000, distances = 3000, distancet = 3000, error = 100;//초음파 계산변수, error는 +-범위(오차);; sense0~3에 사용
int redstate = 0;//붉은색 켜짐 or꺼짐, 처음에는 꺼짐;; doordef에 사용
int selector = 0;
int num0 = 0, num1 = 0, num2 = 0, num3 = 0;
//
void setup() {
  //delay(5000);
  Serial.begin(9600);
  //버튼 인터럽트 안되면 주석 해제
  //pinMode(ledbtn, INPUT);//버튼 입력
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);//1초음파
  pinMode(fechoPin, INPUT);
  pinMode(ftrigPin, OUTPUT);//2초음파
  pinMode(sechoPin, INPUT);
  pinMode(strigPin, OUTPUT);//3초음파
  pinMode(techoPin, INPUT);
  pinMode(ttrigPin, OUTPUT);//4초음파
  //----------------------------------
  pinMode(redlight_out, OUTPUT);//LED RED
  pinMode(whitelight_out, OUTPUT);//LED WHITE
  pinMode(buzzer, OUTPUT);// buzzer
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  attachInterrupt(digitalPinToInterrupt(ledbtn), btninterrupt, LOW);
}

//센서 센싱-------------------------------------------------------------------------
//sense숫자:센싱만 하는거
//sense숫자light:불까지 컨트롤 하는거
int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};
//tone(buzzer, tones[3]);//           buzzer 예제
//noTone(piezo);//                    buzzer 중단
int sense0(){
  long durationz;
  num0 = 0;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // duration은 echo핀이 HIGH를 유지한 시간
  durationz = pulseIn(echoPin, HIGH);
  // distance = (speed * time = 음속(340m/s) * duration)/2;;왕복 거리이므로 2로 나누기! ps 전선 2미터로 길어서 50mm추가로 인식함(50 빼주기)
  distancez = ((float)(340*durationz)/1000)/2;
  if(distancet >= 3000){
    distancez = 3500;
  }
  selectdis();
  return 0;
}

int sense1(){
  long durationf;
  num1 = 0;
  digitalWrite(ftrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ftrigPin, LOW);
  durationf = pulseIn(fechoPin, HIGH);
  distancef = ((float)(340*durationf)/1000)/2;
  if(distancef >= 3000){
    distancef = 3500;
  }
  selectdis();
  return 0;
}

int sense2(){
  long durations;
  num2 = 0;
  digitalWrite(strigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(strigPin, LOW);
  durations = pulseIn(sechoPin, HIGH);
  distances = ((float)(340*durations)/1000)/2;
  if(distances >= 3000){
    distances = 3500;
  }
  selectdis();
  return 0;
}

int sense3(){
  long durationt;
  num3 = 0;
  digitalWrite(ttrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ttrigPin, LOW);
  durationt = pulseIn(techoPin, HIGH);
  distancet = ((float)(340*durationt)/1000)/2;
  if(distancet >= 3000){
    distancet = 3500;
  }
  selectdis();
  return 0;
}

//셀렉터------------------------------------------------------------------------------------------------
void set1(){
  Serial.print("sel1 ");
  sense0();
  distchose(selector, 0);
}

void set2(){
  Serial.print("sel2 ");
  sense1();
  distchose(selector, 1);
}

void set3(){
  Serial.print("sel3 ");
  sense2();
  distchose(selector, 2);
}

void set4(){
  Serial.print("sel4 ");
  sense3();
  distchose(selector, 3);
}

void selectdis(){
  int sela = analogRead(selector1);
  int selb = analogRead(selector2);
  int selc = analogRead(selector3);
  if(sela > 1000 && selb < 300 && selc < 300){
    selector = 0;
  }
  else if(sela < 300 && selb > 1000 && selc < 300){
    selector = 1;
  }
  else if(sela < 300 && selb < 300 && selc > 1000){
    selector = 2;
  }
  else if(sela < 300 && selb < 300 && selc < 300){
    selector = 3;
  }
  Serial.print(analogRead(selector1));
  Serial.print("  ");
  Serial.print(analogRead(selector2));
  Serial.print("  ");
  Serial.print(analogRead(selector3));
  Serial.print("  ");
  Serial.print(selector);
  Serial.print("   |   ");
  Serial.print(distancez);
  Serial.print("  ");
  Serial.print(distancef);
  Serial.print("  ");
  Serial.print(distances);
  Serial.print("  ");
  Serial.println(distancet);
//  Serial.print(" | ");
//  Serial.print(num0);
//  Serial.print("  ");
//  Serial.print(num1);
//  Serial.print("  ");
//  Serial.print(num2);
//  Serial.print("  ");
//  Serial.println(num3);
}

void distchose(int num, int sens){
  if(sens == 0){
    if(num == 0){
      if(distancez <= lowdist + error){
        num0 = 1;
      }
    }
    else if(num == 1){
      if(distancez <= middist + error){
        num0 = 1;
      }
    }
    else if(num == 2){
      if(distancez <= highdist + error){
        num0 = 1;
      }
    }
  }
  else if(sens == 1){
    if(num == 0){
      if(distancef <= lowdist + error){
        num1 = 1;
      }
    }
    else if(num == 1){
      if(distancef <= middist + error){
        num1 = 1;
      }
    }
    else if(num == 2){
      if(distancef <= highdist + error){
        num1 = 1;
      }
    }
  }
  else if(sens == 2){
    if(num == 0){
      if(distances <= lowdist + error){
        num2 = 1;
      }
    }
    else if(num == 1){
      if(distances <= middist + error){
        num2 = 1;
      }
    }
    else if(num == 2){
      if(distances <= highdist + error){
        num2 = 1;
      }
    }
  }
  else if(sens == 3){
    if(num == 0){
      if(distancet <= lowdist + error){
        num3 = 1;
      }
    }
    else if(num == 1){
      if(distancet <= middist + error){
        num3 = 1;
      }
    }
    else if(num == 2){
      if(distancet <= highdist + error){
        num3 = 1;
      }
    }
  }
  if(num0 == 1 or num1 == 1 or num2 == 1 or num3 == 1){
    doordef(2);
    tone(buzzer, tones[7]);
  }
  else{
    noTone(buzzer);
    doordef(1);
  }
}

void doordef(int n){
  if(n==0){//붉은색 꺼짐
    digitalWrite(redlight_out,0);
    digitalWrite(whitelight_out,0);
    redstate = 0;
  }
  else if(n == 1){//붉은색 켜짐
    digitalWrite(redlight_out,1);
    digitalWrite(whitelight_out,0);
    redstate = 1;
  }
  else if(n==2){//붉은색 스위치(상태 바꾸기)
    if(redstate == 0){
      redstate = 1;
      digitalWrite(redlight_out,1);
      digitalWrite(whitelight_out,0);
    }
    else{
      redstate = 0;
      digitalWrite(redlight_out,0);
      digitalWrite(whitelight_out,0);
    }
  }
  delay(100);
}
//tone(buzzer, tones[7]);//           buzzer 예제
//noTone(buzzer);//                   buzzer 중단
//white버튼 인터럽트 함수-----------------------------------------------
void btninterrupt(){
  digitalWrite(redlight_out,0);
  digitalWrite(whitelight_out,1);
  //analogWrite(redlight_out,0);
  //analogWrite(whitelight_out, 255);
  Serial.println("white");
  start = 0;
  noTone(buzzer);
  return 0;
}
//메인 루프------------------------------------------------------------
void loop(){
  noTone(buzzer);
  doordef(1);
  while(1){
    set1();
    set2();
    set3();
    set4();
  }
  
}
