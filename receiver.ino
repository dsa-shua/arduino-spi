#define CLK 2
#define MISO 3
#define MOSI 4
#define CS 5
#define CP 1000


//int d_clk, d_miso, d_mosi, d_cs;
int in_byte[8] = {0,0,0,0,0,0,0,0};
int parity_bit = 0;
int in_char = 0;
char in_string[] = {}; 
int str_idx = 0;

void CLK_EDGE() {
  // rising edge trigger
  if (digitalRead(CLK) == LOW) {
    while(digitalRead(CLK) == LOW){}
  }
  else if (digitalRead(CLK) == HIGH) {
    while(digitalRead(CLK) == HIGH) {}
    while(digitalRead(CLK) == LOW) {}
  }
}


void test_setup() {
  pinMode(CLK, INPUT);
  pinMode(MISO, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(CS, INPUT);
}

void setup() {
  Serial.begin(9600);
}

void test() {
  delay(500);
  Serial.print(">>> ");
  
  if (digitalRead(CLK) == HIGH) {
    Serial.print("1");
  } else {
    Serial.print("0");
  }

  if (digitalRead(MISO) == HIGH) {
    Serial.print("1");
  } else {
    Serial.print("0");
  }

  if (digitalRead(MOSI) == HIGH) {
    Serial.print("1");
  } else {
    Serial.print("0");
  }

  if (digitalRead(CS) == HIGH) {
    Serial.print("1");
  } else {
    Serial.print("0");
  }
  Serial.println("");
}


void decode_char(){
  in_char = (in_byte[0]);
  in_char = in_char + (in_byte[1] << 1);
  in_char = in_char + (in_byte[2] << 2);
  in_char = in_char + (in_byte[3] << 3);
  in_char = in_char + (in_byte[4] << 4);
  in_char = in_char + (in_byte[5] << 5);
  in_char = in_char + (in_byte[6] << 6);
  in_char = in_char + (in_byte[7] << 7);

//  Serial.print("CHAR: ");
  Serial.print((char)in_char);
}

void decode_message() {
  Serial.print("Message: ");
  Serial.println(in_string);
}

void receive() {
  while(digitalRead(MOSI) == HIGH) {} // wait for start bit
//  Serial.print(">>> ");

  // BIT 0
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[0] = 1;} else {in_byte[0] = 0;}
//  Serial.print(in_byte[0]);

  // BIT 1
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[1] = 1;} else {in_byte[1] = 0;}
//  Serial.print(in_byte[1]);

  // BIT 2
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[2] = 1;} else {in_byte[2] = 0;}
//  Serial.print(in_byte[2]);

  // BIT 3
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[3] = 1;} else {in_byte[3] = 0;}
//  Serial.print(in_byte[3]);

  // BIT 4
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[4] = 1;} else {in_byte[4] = 0;}
//  Serial.print(in_byte[4]);

  // BIT 5
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[5] = 1;} else {in_byte[5] = 0;}
//  Serial.print(in_byte[5]);

  // BIT 6
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[6] = 1;} else {in_byte[6] = 0;}
//  Serial.print(in_byte[6]);

  // BIT 7
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {in_byte[7] = 1;} else {in_byte[7] = 0;}
//  Serial.print(in_byte[7]);

  // PARITY BIT
  CLK_EDGE();
  if (digitalRead(MOSI) == HIGH) {parity_bit = 1;} else {parity_bit = 0;}
//  Serial.print("\n PARITY: ");
//  Serial.println(parity_bit);
  
  // END SIGNAL
  CLK_EDGE();
  while (digitalRead(MOSI) == LOW) {}
  decode_char();
}

void loop() {
  while(digitalRead(CS) == LOW) {} // spinlock
  str_idx = 0;
//  Serial.print(">>> ");
  while(digitalRead(CS) == HIGH) {
    receive();  
  }
  Serial.println("");
}
