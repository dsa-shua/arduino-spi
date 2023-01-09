#define CLK 2
#define MISO 3
#define MOSI 4
#define CS 5
#define CP 15
// period in ms

// R: CLK, B: MISO, G: MOSI, Y: CS

char in_char = 0;
int in_byte[8] = {0,0,0,0,0,0,0,0};
int parity_bit = 0;

void setup() {
  Serial.begin(9600);
  pinMode(CLK,OUTPUT);
  pinMode(MISO,OUTPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(CS,OUTPUT);

  for (unsigned int i = 0; i < 5; i ++) {
    test();
  }

  digitalWrite(MOSI,HIGH);
  
  pinMode(MISO, INPUT);
  
}

void test() {
  delay(500);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  delay(500);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
}

void print_bits () {
    Serial.print(">>> ");
    Serial.print(in_char);
    Serial.print(" | ");
    Serial.print(in_byte[7]);
    Serial.print(in_byte[6]);
    Serial.print(in_byte[5]);
    Serial.print(in_byte[4]);
    Serial.print(in_byte[3]);
    Serial.print(in_byte[2]);
    Serial.print(in_byte[1]);
    Serial.println(in_byte[0]);
}

void SPI() {

  // START PHASE
  digitalWrite(CLK, HIGH); digitalWrite(CS,HIGH); delay(1); digitalWrite(MOSI,LOW); 
  delay(CP);
  delay(CP); digitalWrite(CLK, LOW); 

  // BIT 0 PHASE
  if (in_byte[0] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 

  // BIT 1 PHASE
  if (in_byte[1] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 
  
  // BIT 2 PHASE
  if (in_byte[2] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 
  
  // BIT 3 PHASE
  if (in_byte[3] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 
  
  // BIT 4 PHASE
  if (in_byte[4] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 
  
  // BIT 5 PHASE
  if (in_byte[5] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 
  
  // BIT 6 PHASE
  if (in_byte[6] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 
  

  // BIT 7 PHASE
  if (in_byte[7] == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW); 

  // PARITY PHASE
  if (parity_bit == 0){digitalWrite(MOSI,LOW);} else {digitalWrite(MOSI,HIGH);}
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW);

  // END PHASE
  digitalWrite(MOSI,HIGH);
  delay(CP); digitalWrite(CLK, HIGH); 
  delay(CP); digitalWrite(CLK, LOW);
}

int parity(int which) {
  parity_bit = in_byte[0];
  for (unsigned int i = 1; i < 8; i++) {
    parity_bit  = in_byte[i] ^ parity_bit;
  }
  return parity_bit;
  // if return 1: odd, 0: even
}

void transmit() {
  in_char = Serial.read();
  if (1) {
    
    // 0: LSB, 7: MSB
    in_byte[0] = (int)in_char & 0b1;
    in_byte[1] = ((int)in_char & 0b10) >> 1;
    in_byte[2] = ((int)in_char & 0b100) >> 2;
    in_byte[3] = ((int)in_char & 0b1000) >> 3;
    in_byte[4] = ((int)in_char & 0b10000) >> 4;
    in_byte[5] = ((int)in_char & 0b100000) >> 5;
    in_byte[6] = ((int)in_char & 0b1000000) >> 6;
    in_byte[7] = ((int)in_char & 0b10000000) >> 7;

    parity_bit = parity((int)in_char);
    
    print_bits();
    
    SPI(); // do SPI PROTOCOL
  }
}


void start(int which) {
  if (which == 0) { // the slave?
    delay(10);
    digitalWrite(MOSI,HIGH);
    transmit(); // the actual transmission procedure   
  }
}

void loop() {
  if (Serial.available() > 0) {
    start(0);
  }
  digitalWrite(CS,LOW);
}
