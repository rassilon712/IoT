#include <DHT11.h>
#include <Servo.h>


int a,b,c;
int angle = 0;
bool alarm = true;
bool menu = false;
int pin_DUST = 5;
int pin_DHT11 = 6;
int pin_SERVO = 9;
int pin_BUZZER = 35;
int pin_LED[3] = {11, 12, 13}; // R, G, B



Servo servo; 
DHT11 dht11(pin_DHT11); 

void setup() {
  // Bluetooth(UART3) setup baud 9600, data bit 8, parity None, stop bit 1
  Serial3.begin(9600);  // same Serial3.begin(9600, SERIAL_8N1) Bloo
  Serial.begin(9600);
  pinMode(pin_BUZZER, OUTPUT);
  pinMode(pin_DUST, OUTPUT);
  pinMode(pin_LED[0], OUTPUT);
  pinMode(pin_LED[1], OUTPUT);
  pinMode(pin_LED[2], OUTPUT);

  servo.attach(pin_SERVO); 

}

void loop() {
  // When receiving Bluetooth data
  if(Serial3.available() > 0){//test

  if(alarm == true){
    if(Serial3.available() > 0){
      digitalWrite(pin_BUZZER, HIGH);
      print_alarm();
      bool correct = false;
      
      while(!correct){
          if(Serial3.available() > 0){
         
          String ans_c;
          while(Serial3.available()){
            char mychar = Serial3.read();
            ans_c += mychar;
            delay(10);
          }
          int ans = atoi(ans_c.c_str());
          correct = solve_alarm(ans);
        }
      }
      digitalWrite(pin_BUZZER, LOW);
      alarm = false;
      menu = true;
    }
  }
  int num;
  if(menu) num = print_menu();

  while(menu){ // don't delete
  }
  
  if (num==1) {
    print_wheather();
    menu = true;
  }
  else if (num==2) {
    get_coffee();
    menu = true;
  }
  else if (num==3){
   get_rate();
   menu = true; 
  }
  else Serial3.println("That is a wrong request.");
  }//test
}


void print_alarm(){
    a = random(10);
    b = random(10);
    c = random(100);
    String a_string = String(a);
    String b_string = String(b);
    String c_string = String(c);
    Serial3.write("***** Good Morning! Mr.LEE! If you want to stop the alarm, Answer the following question.\n");
    Serial3.write("Qusetion : ");
    Serial3.write(a_string.c_str());
    Serial3.write(" * ");
    Serial3.write(b_string.c_str());
    Serial3.write(" + ");
    Serial3.write(c_string.c_str());
    Serial3.write(" = ???\n");
}

bool solve_alarm(int ans){
  int answer = a*b+c;
  if (answer == ans) {
    Serial3.write("Correct! The alarm is stopped. Have a nice day!\n");
    return true;
  }
  else {
    Serial3.write("Your answer is wrong. Please write your new answer again.\n");
    return false;
  }
}

int print_menu(){
  int num;
  Serial3.write("**************MENU**************\n");
  Serial3.write("1. Today's wheather\n");
  Serial3.write("2. Get some coffee\n");
  Serial3.write("3. Get Discomfort Index\n\n");
  while(true){
     String mystring;

     while(Serial3.available()){
          char mychar = Serial3.read();
          mystring += mychar;
          delay(10);
      }
      num = atoi(mystring.c_str());
      menu = false;
      if(num==1) break;
      else if(num==2) break;
      else if(num==3) break;
  }
  return num;
}

void print_wheather(){
  float dust, temp, humi;
  int err;


  dust = analogRead(pin_DUST);

  if((err=dht11.read(humi, temp))==0)
  {
    Serial3.print("*********Today Wheather*********\n\n");
    Serial3.print("Dust concentration: ");
    Serial3.print(dust);
    Serial3.println(" ug/m3");
    Serial3.print("Temperature: ");
    Serial3.print(temp);
    Serial3.println(" 'C");
    Serial3.print("Humidity: ");
    Serial3.print(humi);
    Serial3.print(" %\n\n");
  }
  else
  {
    Serial3.println();
    Serial3.print("Error No :");
    Serial3.print(err);
    Serial3.println();    
  }
  delay(DHT11_RETRY_DELAY);
}

void get_coffee(){
  Serial3.print("*****Coffee Machine Operating*****\n\n");

  for(angle = 0; angle < 180; angle++) 
  { 
    servo.write(angle); 
    delay(15); 
  } 
  delay(1000);
  for(angle = 180; angle > 0; angle--) 
  { 
    servo.write(angle); 
    delay(15); 
  } 
}

void get_rate(){
  float rate,temp,humi;
  int err;
  err = dht11.read(humi, temp);
  rate = 9.0/5.0*temp - 0.55*(1-humi/100)*(9.0/5.0*temp-26)+32;
  Serial3.print("**********Discomfort index**********\n\n");
  Serial3.print("Discomfort index: ");
  Serial3.println(rate);
  Serial3.print("\n");
  if (rate < 72.0){
      digitalWrite(pin_LED[2],1);
      delay(5000);
      digitalWrite(pin_LED[2],0);
      }
  else if (rate>=72.0 && rate <78.0){
      digitalWrite(pin_LED[1],1);
      delay(5000);
      digitalWrite(pin_LED[1],0);
      }
  else if (rate>=78.0){
      digitalWrite(pin_LED[0],1);
      delay(5000);
      digitalWrite(pin_LED[0],0);
      }
}

