/*
 * This is a dummy program that reads from the serial commands and executes them
*/



#define HANDSHAKE  0
#define  SET  1
#define  GET  2
#define  DIGITAL_FLAG 1

void setup() {
  for(int i = 2; i <=13; ++i){
    pinMode(i, OUTPUT);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(5000);
  Serial.println("Hello Computer");
}



bool led_is_on=false;

void SplitUint32(uint32_t &num,
                 uint8_t &out_byte1,
                 uint8_t &out_byte2,
                 uint8_t &out_byte3,
                 uint8_t &out_byte4) {
  out_byte1 = num & 0xFF;
  out_byte2 = (num >> 8) & 0xFF;
  out_byte3 = (num >> 16) & 0xFF;
  out_byte4 = (num >> 24) & 0xFF;
}


void loop() {
  
  uint8_t buffer[4];
  while(Serial.available()>=4){
    Serial.readBytes(buffer, 4);
    uint8_t & pin = buffer[0];
    uint8_t & command = buffer[1];
    uint8_t & flags = buffer[2];
    uint8_t & value = buffer[3];

    if(pin == 0xFF)
         pin = LED_BUILTIN;

    if(command == SET){
      command = GET;
       if(flags & DIGITAL_FLAG){
          digitalWrite(pin, value);
          value = digitalRead(pin);
       } else {
          analogWrite(pin, value);
          value = analogRead(pin);
       }
       Serial.write(buffer, 4);         
    }
    else if(command == GET){
      command = GET;
       if(flags & DIGITAL_FLAG){          
          value = digitalRead(pin);
       } else {          
          value = analogRead(pin);
       }
       Serial.write(buffer, 4);         
    }
  }
    
   /*
    uint8_t command_type = command & 0xFF;
    String  output[4];
    output[0] = String(command_type);
    if(command_type & 1){ // Set command
      uint8_t pin = (command >> 8) & 0xFF;
      if(pin = 0xFF)
         pin = LED_BUILTIN;
      output[1] = String(pin);
      uint8_t velocity = (command >> 16) & 0xFF;
      output[2] = String(velocity);
      if(command_type & 128)
         digitalWrite(pin, velocity);
      else
         analogWrite(pin, velocity);
      Serial.write(output[0].c_str());
      Serial.write(',');
      Serial.write(output[1].c_str());
      Serial.write(',');
      Serial.write(output[2].c_str());
    }
  }
   */

  // put your main code here, to run repeatedly:

}
