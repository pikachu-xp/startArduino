#include "Keyboard.h"
#include "pins_arduino.h" // Arduino pre-1.0 needs this

char tabKey = KEY_TAB;
char up = KEY_UP_ARROW;
char down = KEY_DOWN_ARROW;
char left = KEY_LEFT_ARROW;
char right = KEY_RIGHT_ARROW;

uint8_t readCapacitivePin(int pinToMeasure) {
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  byte bitmask;

  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  delay(1);

  uint8_t SREG_old = SREG;

  noInterrupts();

  *ddr &= ~(bitmask);
  *port |= bitmask;

  uint8_t cycles = 17;

  if (*pin & bitmask) {
    cycles =  0;
  }

  else if (*pin & bitmask) {
    cycles =  1;
  }

  else if (*pin & bitmask) {
    cycles =  2;
  }

  else if (*pin & bitmask) {
    cycles =  3;
  }

  else if (*pin & bitmask) {
    cycles =  4;
  }

  else if (*pin & bitmask) {
    cycles =  5;
  }

  else if (*pin & bitmask) {
    cycles =  6;
  }

  else if (*pin & bitmask) {
    cycles =  7;
  }

  else if (*pin & bitmask) {
    cycles =  8;
  }

  else if (*pin & bitmask) {
    cycles =  9;
  }

  else if (*pin & bitmask) {
    cycles = 10;
  }

  else if (*pin & bitmask) {
    cycles = 11;
  }

  else if (*pin & bitmask) {
    cycles = 12;
  }

  else if (*pin & bitmask) {
    cycles = 13;
  }

  else if (*pin & bitmask) {
    cycles = 14;
  }

  else if (*pin & bitmask) {
    cycles = 15;
  }

  else if (*pin & bitmask) {
    cycles = 16;
  }

  SREG = SREG_old;

  *port &= ~(bitmask);

  *ddr  |= bitmask;

  return cycles;
}

int limiar = 3;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Keyboard.releaseAll();
  
  while (readCapacitivePin(7) > limiar) {
    Serial.println("Cima");
    cima();
  }

  Keyboard.releaseAll();
  
  while (readCapacitivePin(6) > limiar) {
    Serial.println("Baixo");
    baixo();
  }

  Keyboard.releaseAll();

  while (readCapacitivePin(5) > limiar) {
    Serial.println("Direita");
    direita();
  }

  Keyboard.releaseAll();

  while (readCapacitivePin(4) > limiar) {
    Serial.println("Esquerda");
    esquerda();
  }

  Keyboard.releaseAll();

  while (readCapacitivePin(3) > limiar) {
    Serial.println("Tab");
    tab();
  }

  Keyboard.releaseAll();
}

void esquerda() {
  Keyboard.press(left);
  delay(100);
  //Keyboard.releaseAll();
}

void direita() {
  Keyboard.press(right);
  delay(100);
  //Keyboard.releaseAll();
}

void tab() {
  Keyboard.press(tabKey);
  delay(100);
  //Keyboard.releaseAll();
}

void cima() {
  Keyboard.press(up);
  delay(100);
  //Keyboard.releaseAll();
} 
      
void baixo() {
  Keyboard.press(down);
  delay(100);
  //Keyboard.releaseAll();
}
