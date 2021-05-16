#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 32, &Wire, 4);

volatile uint32_t counter;
volatile uint32_t secunds;

void setup() {
  counter = 0;
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(24,0);
  display.println("Geiger");
  display.setCursor(17,15);
  display.print("Counter");
  display.display();
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), count, HIGH);
  timersetup();
  secunds = 0;
}

void timersetup(){
cli();
TCCR1A = 0;
TCCR1B = 0;
TCNT1  = 0;
OCR1A = 62499;
TCCR1B |= (1 << WGM12);
TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
TIMSK1 |= (1 << OCIE1A);
sei();
}

void loop() {
  if(secunds >= 60){
    display_count();
  }
  update_progressbar();
}

void display_count(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);
  display.print(counter * 0.00812);
  display.setTextSize(2);
  display.print(" uS/h");
  counter = 0;
  secunds = 0;
  display.display();
}

ISR(TIMER1_COMPA_vect)
{
  secunds += 1;
}

void update_progressbar(){
  display.drawFastHLine(0, 31, round(secunds*2.133333), SSD1306_WHITE);
  display.display();
}

void count(){
  counter++;
}
