// platform = espressif32
// board = az-delivery-devkit-v4
// framework = arduino
// monitor_speed = 115200
// monitor_rts = 0

#include "BluetoothSerial.h" 

BluetoothSerial espBT; 

int relay_status(int);
int switch_bit(int);

const int relay_K1 = 4;
const int relay_K2 = 16;
const int relay_K3 = 17;
const int relay_K4 = 18;

void setup() {
  Serial.begin(115200);
  pinMode(relay_K1, OUTPUT);
  pinMode(relay_K2, OUTPUT);
  pinMode(relay_K3, OUTPUT);
  pinMode(relay_K4, OUTPUT);

  // turn all relays of
  digitalWrite(relay_K1, HIGH);
  digitalWrite(relay_K2, HIGH);
  digitalWrite(relay_K3, HIGH);
  digitalWrite(relay_K4, HIGH);

  espBT.begin("ESP32 Bluetooth");
  Serial.println("Pair your BT device now");  
}

void loop(){
  //output of ESP32 to be switched
  int pin = 0;
  // the bluetooth data gets written into buffer. If data arrives, 
  // espBT.available() evaluates to the number of bytes in buffer. 
  // 
  if(espBT.available()){
    int relay = espBT.parseInt();
    // check for correct input 
    if(1<= relay <= 4){
     
      // match Relay to Pin
      if (relay == 1) {
        pin = relay_K1;
      }
      if (relay == 2) {
        pin = relay_K2;
      }
      if (relay == 3) {
        pin = relay_K3;
      }
      if (relay == 4) {
        pin = relay_K4;
      }

      // activate relay
      digitalWrite(pin, switch_bit(digitalRead(pin)));

      // communicate changes
      espBT.print("switched K"); 
      espBT.println(relay); 
      espBT.print(", status: ");
      espBT.print(relay_status(relay_K1));
      espBT.print(relay_status(relay_K2));
      espBT.print(relay_status(relay_K3));
      espBT.print(relay_status(relay_K4));   
       
      // empty the serial buffer by reading everything from it
      // data send from phone terminates with \n (newline character) and espBT.parseInt() returns 0 when 
      // reading \n. To avoid this we read the buffer til its empty 
      while(espBT.available() > 0) espBT.read(); 
    } // end if(1<= relay <= 4)
    else{
      espBT.print("wrong input. Accepted are: \"1\", \"2\", \"3\" or \"4\".");
      delay(100);
    } 
  } // end if(espBT.available())
  delay(20);     
}

int relay_status(int pin){
  //relays have low-level-trigger
  int status = digitalRead(pin);
  if (status == HIGH){
    return 0;
  }
  if (status == LOW){
    return 1;
  }
  else {
    return -1;
  }
}


int switch_bit(int current_state){
  int new_state;
  if (current_state == 0){
    new_state = 1;
  }
  else if (current_state == 1){
    new_state = 0;
  }
  else {
    new_state = -1;
  }
  return new_state;
}