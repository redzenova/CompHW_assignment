// Library
#include <Keypad.h>

//Motor Input signals FPGA to Arduino
#define m1_ops_in_PIN 63
#define m2_ops_in_PIN 59
#define m3_ops_in_PIN 65
#define m4_ops_in_PIN 66

// Vending Motor
// X Motor
#define m1_dir_PIN  55
#define m1_step_PIN 54
#define m1_en_PIN   38

// Y Motor
#define m2_dir_PIN  61
#define m2_step_PIN 60
#define m2_en_PIN   56

// Z Motor
#define m3_dir_PIN  48
#define m3_step_PIN 46
#define m3_en_PIN   62

// E0 Motor
#define m4_dir_PIN  28
#define m4_step_PIN 26
#define m4_en_PIN   24

// Coin Arduino out to FPGA
#define OUT_C1_PIN   53
#define OUT_C2_PIN   52
#define OUT_C5_PIN   51
#define OUT_C10_PIN  50
#define OUT_C_TK_PIN 49

#define stepsPerRevolution 200

boolean m1_state = false;
boolean m2_state = false;
boolean m3_state = false;
boolean m4_state = false;

// Numpad Arduino out to FPGA
#define OUT_N1_PIN   37
#define OUT_N2_PIN   39
#define OUT_N3_PIN   41
#define OUT_N4_PIN   43
#define OUT_NCLK_PIN 45
#define OUT_NE_PIN   47

#define sensor 2 // ตัวแปรของเครื่องรับเหรียญทำงานที่ขา 2

// Keypad
void inputKeypad();

const byte ROWS = 5;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'\0', '\0', '\0', '\0'},
  {'1', '2', '3', '\0'},
  {'4', '5', '6', '\0'},
  {'7', '8', '9', '\0'},
  {'\0', '0', '\0', 'E'},
};

char selected = ' ';

byte rowPins[ROWS] = {16, 17, 23, 25, 27};
byte colPins[COLS] = {35, 33, 31, 29};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int i = 0;
int k = 0; // ตัวแปรที่ใช้แทนมูลค่าเหรียญ ในที่นี้ใช้แทนเหรียญ 10 บาท สามารถเปลี่ยนได้
int kc = 0; // ตัวแปรที่ใช้แทนขนิดเหรียญ ในที่นี้ใช้แทนเหรียญ 10 บาท สามารถเปลี่ยนได้
int s = 0; // ตัวแปรที่ใช้แทนมูลค่าเหรียญ ในที่นี้ใช้แทนเหรียญ 1 บาท สามารถเปลี่ยนได้
int sc = 0; // ตัวแปรที่ใช้แทนขนิดเหรียญ ในที่นี้ใช้แทนเหรียญ 1 บาท สามารถเปลี่ยนได้
int p = 0; // ตัวแปรที่ใช้แทนมูลค่าเหรียญ ในที่นี้ใช้แทนเหรียญ 2 บาท สามารถเปลี่ยนได้
int pc = 0; // ตัวแปรที่ใช้แทนขนิดเหรียญ ในที่นี้ใช้แทนเหรียญ 2 บาท สามารถเปลี่ยนได้
int j = 0; // ตัวแปรที่ใช้แทนมูลค่าเหรียญ ในที่นี้ใช้แทนเหรียญ 5 บาท สามารถเปลี่ยนได้
int jc = 0; // ตัวแปรที่ใช้แทนขนิดเหรียญ ในที่นี้ใช้แทนเหรียญ 5 บาท สามารถเปลี่ยนได้
int a = 0; // ตัวแปรสำหรับเก็บผลรวมจำนวนเงินทั้งหมด สามารถเปลี่ยนได้
int i10 = 0; // ตัวแปรสำหรับนับจำนวนเหรียญแต่ละชนิด ในที่นี้ใช้แทนเหรียญ 10 บาท สามารถเปลี่ยนได้
int i5 = 0; // ตัวแปรสำหรับนับจำนวนเหรียญแต่ละชนิด ในที่นี้ใช้แทนเหรียญ 5 บาท สามารถเปลี่ยนได้
int i2 = 0; // ตัวแปรสำหรับนับจำนวนเหรียญแต่ละชนิด ในที่นี้ใช้แทนเหรียญ 5 บาท สามารถเปลี่ยนได้
int i1 = 0; // ตัวแปรสำหรับนับจำนวนเหรียญแต่ละชนิด ในที่นี้ใช้แทนเหรียญ 1 บาท สามารถเปลี่ยนได้

void setup() {
  pinMode(OUT_C1_PIN, OUTPUT);
  pinMode(OUT_C2_PIN, OUTPUT);
  pinMode(OUT_C5_PIN, OUTPUT);
  pinMode(OUT_C10_PIN, OUTPUT);
  pinMode(OUT_C_TK_PIN, OUTPUT);

  pinMode(OUT_N1_PIN, OUTPUT);
  pinMode(OUT_N2_PIN, OUTPUT);
  pinMode(OUT_N3_PIN, OUTPUT);
  pinMode(OUT_N4_PIN, OUTPUT);
  pinMode(OUT_NE_PIN, OUTPUT);
  pinMode(OUT_NCLK_PIN, OUTPUT);

  pinMode(m1_dir_PIN, OUTPUT);
  pinMode(m1_step_PIN, OUTPUT);
  pinMode(m1_en_PIN, OUTPUT);

  pinMode(m2_dir_PIN, OUTPUT);
  pinMode(m2_step_PIN, OUTPUT);
  pinMode(m2_en_PIN, OUTPUT);

  pinMode(m3_dir_PIN, OUTPUT);
  pinMode(m3_step_PIN, OUTPUT);
  pinMode(m3_en_PIN, OUTPUT);

  pinMode(m4_dir_PIN, OUTPUT);
  pinMode(m4_step_PIN, OUTPUT);
  pinMode(m4_en_PIN, OUTPUT);

  pinMode(m1_ops_in_PIN, INPUT);
  pinMode(m2_ops_in_PIN, INPUT);
  pinMode(m3_ops_in_PIN, INPUT);
  pinMode(m4_ops_in_PIN, INPUT);

  pinMode(sensor, INPUT);
  attachInterrupt(0, doCounter, FALLING);
  Serial.begin(9600);
}
boolean isCounter = false;
int count = 0;

void loop() {

  inputKeypad();

  if (isCounter == true) {
    isCounter = false;
    //Serial.println(count);
    delay(500);
  }
  if (i != 800 && count != 0 ) {
    i = i + 1;
  }
  if (i == 800) {
    i = 0;
    if (count == 10) {
      i10 = i10 + 1;
      k = i10 * 10; // มูลค่าเหรียญ
      kc = 10; // ขนิดเหรียญ
      //Serial.print("ชนิดเหรียญล่าสุด: ");
      Serial.print(kc); Serial.println(" บาท");
      //Serial.println();

      digitalWrite(OUT_C10_PIN, HIGH);

    }
    if (count == 5) {
      i5 = i5 + 1;
      j = i5 * 5;
      jc = 5;
      //Serial.print("ชนิดเหรียญล่าสุด: ");
      Serial.print(jc); Serial.println(" บาท");
      //Serial.println();

      digitalWrite(OUT_C5_PIN, HIGH);
    }
    if (count == 2) {
      i2 = i2 + 1;
      p = i2 * 2;
      pc = 2;
      //Serial.print("ชนิดเหรียญล่าสุด: ");
      Serial.print(pc); Serial.println(" บาท");
      //Serial.println();

      digitalWrite(OUT_C2_PIN, HIGH);
    }
    if (count == 1) {
      i1 = i1 + 1;
      s = i1 * 1;
      sc = 1;
      //Serial.print("ชนิดเหรียญล่าสุด: ");
      Serial.print(sc); Serial.println(" บาท");
      //Serial.println();

      digitalWrite(OUT_C1_PIN, HIGH);
    }
    //    Serial.print("   รวมชนิดเหรียญ 10 บาท: ");
    //    Serial.print(i10);Serial.println(" เหรียญ");
    //    Serial.print("   รวมชนิดเหรียญ  5 บาท: ");
    //    Serial.print(i5);Serial.println(" เหรียญ");
    //    Serial.print("   รวมชนิดเหรียญ  2 บาท: ");
    //    Serial.print(i2);Serial.println(" เหรียญ");
    //    Serial.print("   รวมชนิดเหรียญ  1 บาท: ");
    //    Serial.print(i1);Serial.println(" เหรียญ");
    //    Serial.println();
    a = s + k + j + p;
    //    Serial.print("         ยอดเงินรวมทั้งหมด: ");
    //    Serial.print(a);Serial.println(" บาท");
    //    Serial.println();
    count  = 0;
    delay(100);
    digitalWrite(OUT_C_TK_PIN, HIGH);
    delay(50);
    digitalWrite(OUT_C_TK_PIN, LOW);

    digitalWrite(OUT_C1_PIN, LOW);
    digitalWrite(OUT_C2_PIN, LOW);
    digitalWrite(OUT_C5_PIN, LOW);
    digitalWrite(OUT_C10_PIN, LOW);
  }

  if (m1_state == true) {
    motor_start(m1_step_PIN);
  }

  else if (m2_state == true) {
    motor_start(m2_step_PIN);
  }

  else if (m3_state == true) {
    motor_start(m3_step_PIN);
  }

  else if (m4_state == true) {
    motor_start(m4_step_PIN);
  }

}


void doCounter() {
  isCounter = true;
  count++;
}

void inputKeypad()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    Serial.println(key);
    // Send selected item

    if (key == '1')
    {
      digitalWrite(OUT_N1_PIN, HIGH);
      delay(100);
      digitalWrite(OUT_NCLK_PIN, HIGH);
      delay(50);
      digitalWrite(OUT_NCLK_PIN, LOW);
      digitalWrite(OUT_N1_PIN, LOW);
    }

    if (key == '2')
    {
      digitalWrite(OUT_N2_PIN, HIGH);
      delay(100);
      digitalWrite(OUT_NCLK_PIN, HIGH);
      delay(50);
      digitalWrite(OUT_NCLK_PIN, LOW);
      digitalWrite(OUT_N2_PIN, LOW);
    }

    if (key == '3')
    {
      digitalWrite(OUT_N3_PIN, HIGH);
      delay(100);
      digitalWrite(OUT_NCLK_PIN, HIGH);
      delay(50);
      digitalWrite(OUT_NCLK_PIN, LOW);
      digitalWrite(OUT_N3_PIN, LOW);
    }

    if (key == '4')
    {
      digitalWrite(OUT_N4_PIN, HIGH);
      delay(100);
      digitalWrite(OUT_NCLK_PIN, HIGH);
      delay(50);
      digitalWrite(OUT_NCLK_PIN, LOW);
      digitalWrite(OUT_N4_PIN, LOW);
    }


    if (key == 'E')
    {
      //      if (selected == '1') {
      //        digitalWrite(OUT_N1_PIN, HIGH);
      //        m1_state = true;
      //      }
      //      if (selected == '2') {
      //        digitalWrite(OUT_N2_PIN, HIGH);
      //        m2_state = true;
      //      }
      //      if (selected == '3') {
      //        digitalWrite(OUT_N3_PIN, HIGH);
      //        m3_state = true;
      //      }
      //      if (selected == '4') {
      //        digitalWrite(OUT_N4_PIN, HIGH);
      //        m4_state = true;
      //      }
      //      delay(150);
      digitalWrite(OUT_NE_PIN, HIGH);
      delay(250);
      motor_ops();
      delay(250);
      digitalWrite(OUT_NE_PIN, LOW);

      //      digitalWrite(OUT_N1_PIN, LOW);
      //      digitalWrite(OUT_N2_PIN, LOW);
      //      digitalWrite(OUT_N3_PIN, LOW);
      //      digitalWrite(OUT_N4_PIN, LOW);
      selected = ' ';
    }

    else
    {
      selected = key;
    }
  }
}

void motor_start(int m_in_pin) {
  if (m_in_pin == m1_step_PIN) digitalWrite(m1_dir_PIN, HIGH);
  if (m_in_pin == m2_step_PIN) digitalWrite(m2_dir_PIN, LOW);
  if (m_in_pin == m3_step_PIN) digitalWrite(m3_dir_PIN, LOW);
  if (m_in_pin == m4_step_PIN) digitalWrite(m4_dir_PIN, LOW);

  for (int ro = 0; ro < 16; ro++) {
    for (int i = 0; i < stepsPerRevolution; i++) {
      digitalWrite(m_in_pin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(m_in_pin, LOW);
      delayMicroseconds(1000);
    }
  }
  if (m_in_pin == m1_step_PIN) m1_state = false;
  if (m_in_pin == m2_step_PIN) m2_state = false;
  if (m_in_pin == m3_step_PIN) m3_state = false;
  if (m_in_pin == m4_step_PIN) m4_state = false;
}

void motor_ops() {
  if (digitalRead(m1_ops_in_PIN) == HIGH) {
    m1_state = true;
    Serial.println("M1--");
  }

  if (digitalRead(m2_ops_in_PIN) == HIGH) {
    m2_state = true;
    Serial.println("M2--");
  }

  if (digitalRead(m3_ops_in_PIN) == HIGH) {
    m3_state = true;
    Serial.println("M3--");
  }

  if (digitalRead(m4_ops_in_PIN) == HIGH) {
    m4_state = true;
    Serial.println("M4--");
  }
}
