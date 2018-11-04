//D10 управление сервоприводом
//D2 включение сервопривода
//D3 включение электродвигателя
//D4 on/off
//А0 датчик света
//А1 тумблер
#include <Servo.h> //используем библиотеку для работы с сервоприводом

Servo servo;

const int pinPhoto = A0;   
const int pinSwitch = A1;
int pinPh=0;
int pinSw=0;
int main_button=0;
int Secondary_button=0;
int photo_button=0;
int _time=0;

#define engine_on 3
#define on_off 4

void setup() {
  pinMode( pinPhoto, INPUT );
  pinMode( pinSwitch, INPUT );
  pinMode(engine_on, OUTPUT); 
  pinMode(on_off, OUTPUT);
  digitalWrite(on_off,HIGH);
  servo.attach(10); //привязываем привод к порту 10
}

void loop() {
  pinPh = analogRead(pinPhoto);   //данные с фоторестора
  delay(100);

  if(pinPh<900){            //фоторезистор определил приближение к тумблеру
    danger();
  }

  pinSw = analogRead(pinSwitch);  //данные с тумблера
  
  if(pinSw==0){                 //если тумблер включен
    switch_off();               //выбор алгоритма выключения в зависимоти от колличества включений
  }

  if(photo_button==1){       //если фоторезстор определил приближение, но тумблер не включен
    switch(main_button){
      case 5:{
        servo_off;
        break;
        }

      case 8:{
        servo_off;
        break;
        }
      }  
    photo_button=0;
  }
  
  if(main_button==10){    //тумблер вкючен более 15 раз, алгоритм начать сначала
     main_button=0;
     Secondary_button=0;
  }
}

void my_blink(){                           //мигание светодиодом
  for(int i=0; i<10; i++){
    digitalWrite(on_off,LOW);
    delay(200);
    digitalWrite(on_off,HIGH);
  }
}

void switch_off(){                    //выбор алгоритма выключения в зависимости от колличества включений
  switch(main_button){
      case 1:{          //Выключить сразу
        turn_off();
        break;
        }
        
      case 2:{          //Выключить с задержкой 3 секунды
        delay(3000); 
        turn_off();
        break;}
        
      case 3:{          //Высунуть язычок потом выключить
        if(pinSw!=0){
          servo_off();
        }
        open_up();
        delay(1000);
        turn_off();
        break;
        }
        
      case 4:{          //Высунуть язычек, спрятать, потом выключить
        if(pinSw!=0){
         servo_off();
        }
        open_up();
        servo_off();
        turn_off();
        break;
        }
        
      case 5:{            //покрутиться вокруг оси и выключить
        engine();
        turn_off();
        break;
        }
        
      case 6:{          //покрутиться вокруг оси подождать 2 секунды и выключить
        engine();
        delay(2000);
        turn_off();
        break;
        }
        
      case 7:{            //помигать светодиодом и выключить
        my_blink();
        turn_off();
        break;
        }
        
      case 8:{          //помигать светодиодом, покрутиться вокруг оси и выключить
        my_blink();
        engine();
        turn_off();        
        break;
        }
        
      case 9:{        //покрутиться вокрус оси, подождать 2 секунды, опять покрутиться и выключить
        engine();
        delay(2000);
        engine();
        turn_off();
        break;
        }
        
      case 10:{     //покрутиться вокруг оси, высунуть язычок, спрятать его и выключить
        engine();
        
        if(pinSw!=0){
         servo_off();
        }
        
        open_up();
        servo_off();
        turn_off(); 
        break;
        }
      }  
 }

void turn_off(){     //выключение тумблера
  if(pinSw!=0){
    servo_off();
  }
  else{   
  analogWrite(2,HIGH);   //включение севопривода
  servo.write(100);
  delay(3000);
  servo.write(0);
  delay(3000);
  digitalWrite(2,LOW);

  photo_button=1;      //переменная для фоторезистра 
  Secondary_button++;  //первые 5 включений
  if(Secondary_button>=5){
    main_button++;
  }
  else{
    main_button=0;
  }
 }
}

void servo_off(){              //установить сервопривод под 0
  analogWrite(2,HIGH);
  servo.write(0);
  delay(3000);
  analogWrite(2,LOW);
}

void open_up(){                  //высунуть язычок, тумблер не прогать
    analogWrite(2,HIGH);
    servo.write(80);
    delay(3000);
    digitalWrite(2,LOW);
}

void engine(){           //покрутиться вокруг оси
  
 if(pinSw==0){
   digitalWrite(engine_on,HIGH);
   delay(2000);
   digitalWrite(engine_on,LOW);
   main_button++;
 }
 
}

void danger(){                   //фоторезистор распознал приближение и функция определяет что делать в зависимости от колличества включений
  switch(main_button){
    case 3:{                 //покрутиться вокруг оси
      engine();
      break;
      }
    case 5:{                //высунуть язычок
      open_up();
      break;
      }
    case 6:{              //покрутиться вокруг оси
      engine();
      break;
      }
    case 7:{            //покрутиться вокруг оси
      engine();
      break;
      }
    case 8:{             //высунуть язычок
      open_up();
      break;
      }
  }
  delay(2000);
}
