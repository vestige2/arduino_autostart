#include <Bounce2.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

int but = 13;
int led = 12;
unsigned long down = 0;
unsigned long up = 0;
unsigned long prev_up = 0;

boolean ready_to_save1 = false;
boolean ready_to_save2 = false;
boolean ready_to_save3 = false;
boolean ready_to_save_all = false;


boolean ready_to_start = true;
boolean ready_to_pin1 = false;
boolean ready_to_pin2 = false;
boolean ready_to_pin3 = false;


boolean ready_to_change = true;
boolean ready_to_change1 = false;
boolean ready_to_change2 = false;
boolean ready_to_change3 = false;
boolean ready_to_change_all = false;
boolean ready_to_all_pin = false;
int change_count = 0;
int pin1 = -1;
int pin2 = 0;
int pin3 = 0;

int e_pin1;
int e_pin2;
int e_pin3;
int not_pin;

Bounce b = Bounce();

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(but, INPUT);
  digitalWrite(led, HIGH);
  b.attach(but);
  b.interval(5);
  Serial.begin(57600); 
  not_pin = EEPROM.read(20);
  if (not_pin != 38){
    EEPROM.write(20, 38);
    EEPROM.write(16, 1);
    EEPROM.write(17, 2);
    EEPROM.write(18, 3);
    }
  e_pin1 = EEPROM.read(16);
  e_pin2 = EEPROM.read(17);
  e_pin3 = EEPROM.read(18);
  Serial.println(e_pin1);
  Serial.println(e_pin2);
  Serial.println(e_pin3);
}

void loop() {  
  bool updat = b.update();
  unsigned long start = millis();
  main_logic(updat);
  change_logic(updat);  
  // обновляем состояния
  delay(5);
  but_update(start, updat);
  digitalWrite(led, !b.read());
}

void blink(int c, int d) {
  for (int i = 0; i < c; i++) {
    digitalWrite(led, LOW);
    delay(d);
    digitalWrite(led, HIGH);
    delay(d);
  }
}

void but_update(unsigned long start, bool updat) {
  if (!updat && b.read() == 0) {
    // кнопка не нажата вот столько времени
    up = up + millis() - start;
    down = 0;
  }

  if (!updat && b.read() == 1) {
    // кнопка нажата вот столько времени
    down = down + millis() - start;
    up = 0;
  }
}

void change_logic(bool updat){
  if (ready_to_change){
    if (b.read() == 0 && updat) {
      change_count =  change_count + 1;
      Serial.println(change_count);
    }
    if (change_count == 10 && up >= 2000){
    Serial.println("Change pin ready!");
    blink(10, 100);
    ready_to_start = false;    
    ready_to_change = false;
    ready_to_change1 = true;
    pin1 = 0;
    pin2 = 0;
    pin3 = 0;
    change_count = 0;
    }
    if (change_count != 10 && change_count > 0 && up >= 2000){
      Serial.println("Change pin rejected!");
      ready_to_start = true;
      ready_to_change = true;
      change_count = 0;
      }
  }


    // PIN1
  if (ready_to_change1) {
    if (b.read() == 0 && updat) {
      pin1 =  pin1 + 1;
      Serial.println(pin1);
    }
    if (up >= 2000 && pin1 > 0) {
      Serial.println(pin1);
      ready_to_change1 = false;
      Serial.println(up);
      Serial.println("Pin1-stop");
      blink(pin1, 300);
      ready_to_change2 = true;
    }
    if (up >= 6000 && pin1 == 0) {
      ready_to_change1 = false;
      Serial.println(up);
      Serial.println("Pin1-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }



  // PIN2
  if (ready_to_change2) {
    if (b.read() == 0 && updat) {
      pin2 =  pin2 + 1;
      Serial.println(pin2);
    }
    if (up >= 2000 && pin2 > 0) {
      Serial.println(pin2);
      ready_to_change2 = false;
      Serial.println(up);
      Serial.println("Pin2-stop");
      blink(pin2, 300);
      ready_to_change3 = true;
    }
    if (up >= 2000 && pin1 == 0) {
      ready_to_change2 = false;
      Serial.println(up);
      Serial.println("Pin2-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }


  // PIN3
  if (ready_to_change3) {
    if (b.read() == 0 && updat) {
      pin3 =  pin3 + 1;
      Serial.println(pin3);
    }
    if (up >= 2000 && pin3 > 0) {
      Serial.println(pin3);
      ready_to_change3 = false;
      Serial.println(up);
      Serial.println("Pin3-stop");
      blink(pin3, 300);
      ready_to_change_all = true;
      delay(1000);
    }
    if (up >= 2000 && pin1 == 0) {
      ready_to_change3 = false;
      Serial.println(up);
      Serial.println("Pin3-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }
  // Полностью ввели пин
  if (ready_to_change_all) {
    if (pin1 == e_pin1 && pin2 == e_pin2 && pin3 == e_pin3) {
      Serial.println("Pin OK! Ready to save!");
      blink(20, 100);
      ready_to_start = false;
      ready_to_change = false;
      ready_to_change_all = false;
      ready_to_save1 = true;
      pin1 = 0;
      pin2 = 0;
      pin3 = 0;
    }
    else {
      Serial.println("Pin BAD!");
      ready_to_start = true;
      ready_to_change = true;
      ready_to_change_all = false;
    }
  }

    // PIN1
  if (ready_to_save1) {
    if (b.read() == 0 && updat) {
      pin1 =  pin1 + 1;
      Serial.println(pin1);
    }
    if (up >= 2000 && pin1 > 0) {
      Serial.println(pin1);
      ready_to_save1 = false;
      Serial.println(up);
      Serial.println("save1-stop");
      blink(pin1, 300);
      ready_to_save2 = true;
    }
    if (up >= 12000 && pin1 == 0) {
      ready_to_save1 = false;
      Serial.println(up);
      Serial.println("save1-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }
  



  // PIN2
  if (ready_to_save2) {
    if (b.read() == 0 && updat) {
      pin2 =  pin2 + 1;
      Serial.println(pin2);
    }
    if (up >= 2000 && pin2 > 0) {
      Serial.println(pin2);
      ready_to_save2 = false;
      Serial.println(up);
      Serial.println("save2-stop");
      blink(pin2, 300);
      ready_to_save3 = true;
    }
    if (up >= 2000 && pin1 == 0) {
      ready_to_save2 = false;
      Serial.println(up);
      Serial.println("save2-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }


  // PIN3
  if (ready_to_save3) {
    if (b.read() == 0 && updat) {
      pin3 =  pin3 + 1;
      Serial.println(pin3);
    }
    if (up >= 2000 && pin3 > 0) {
      Serial.println(pin3);
      ready_to_save3 = false;
      Serial.println(up);
      Serial.println("save3-stop");
      blink(pin3, 300);
      ready_to_save_all = true;
      delay(1000);
    }
    if (up >= 2000 && pin1 == 0) {
      ready_to_change3 = false;
      Serial.println(up);
      Serial.println("save3-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }

    // Полностью ввели пин
  if (ready_to_save_all) {
      EEPROM.write(16, pin1);
      EEPROM.write(17, pin2);
      EEPROM.write(18, pin3);
      e_pin1 = pin1;
      e_pin2 = pin2;
      e_pin3 = pin3;
      Serial.println("CHANGED_PIN_SUCCESS!!");
      Serial.println("--------------------");
      Serial.print(pin1);
      Serial.print(pin2);
      Serial.print(pin3);
      Serial.println("--------------------");
      ready_to_start = true;
      ready_to_change = true;
      ready_to_save_all = false;    
  }
  
}
  

void main_logic(bool updat) {
  // Готов к вводу пина:  
  if (ready_to_start && down >= 2500) {    
    change_count = 0;
    ready_to_change = false;
    Serial.println("Ready!");
    blink(5, 200);
    ready_to_start = false;
    ready_to_pin1 = true;
    pin1 = -1;
    pin2 = 0;
    pin3 = 0;
  }
  // PIN1
  if (ready_to_pin1) {
    if (b.read() == 0 && updat) {
      pin1 =  pin1 + 1;
      Serial.println(pin1);
    }
    if (up >= 2000 && pin1 > 0) {
      Serial.println(pin1);
      ready_to_pin1 = false;
      Serial.println(up);
      Serial.println("Pin1-stop");
      blink(pin1, 300);
      ready_to_pin2 = true;
    }
    if (up >= 6000 && pin1 == 0) {
      ready_to_pin1 = false;
      Serial.println(up);
      Serial.println("Pin1-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }



  // PIN1
  if (ready_to_pin2) {
    if (b.read() == 0 && updat) {
      pin2 =  pin2 + 1;
      Serial.println(pin2);
    }
    if (up >= 2000 && pin2 > 0) {
      Serial.println(pin2);
      ready_to_pin2 = false;
      Serial.println(up);
      Serial.println("Pin2-stop");
      blink(pin2, 300);
      ready_to_pin3 = true;
    }
    if (up >= 2000 && pin1 == 0) {
      ready_to_pin2 = false;
      Serial.println(up);
      Serial.println("Pin2-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }


  // PIN3
  if (ready_to_pin3) {
    if (b.read() == 0 && updat) {
      pin3 =  pin3 + 1;
      Serial.println(pin3);
    }
    if (up >= 2000 && pin3 > 0) {
      Serial.println(pin3);
      ready_to_pin3 = false;
      Serial.println(up);
      Serial.println("Pin3-stop");
      blink(pin3, 300);
      ready_to_all_pin = true;
      delay(1000);
    }
    if (up >= 2000 && pin1 == 0) {
      ready_to_pin3 = false;
      Serial.println(up);
      Serial.println("Pin3-stop");
      ready_to_start = true;
      ready_to_change = true;
    }
  }
  // Полностью ввели пин
  if (ready_to_all_pin) {
    if (pin1 == e_pin1 && pin2 == e_pin2 && pin3 == e_pin3) {
      Serial.println("Pin OK!");
      blink(20, 100);
      ready_to_start = true;
      ready_to_change = true;
      ready_to_all_pin = false;
    }
    else {
      Serial.println("BAD_PIN!!");
      Serial.println("--------------------");
      Serial.print(pin1);
      Serial.print(pin2);
      Serial.print(pin3);
      Serial.println("--------------------");
      Serial.print(e_pin1);
      Serial.print(e_pin2);
      Serial.print(e_pin3);
      Serial.println("--------------------");
      Serial.println("Pin BAD!");
      ready_to_start = true;
      ready_to_change = true;
      ready_to_all_pin = false;
    }
  }

}
