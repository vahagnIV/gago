/*
 * This is a dummy program that reads from the serial commands and executes them
*/



#define HANDSHAKE = 0
#define  SET = 1
#define  GET = 2
#define TOP_BIT 128

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

void loop() {
  uint32_t command;
  
  while(Serial.available()>=4){
    Serial.readBytes((uint8_t *)&command, sizeof(command));
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

  // put your main code here, to run repeatedly:

}
