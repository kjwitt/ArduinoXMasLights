#include <Adafruit_NeoPixel.h>

#define SPECTRUM_RESET 5 //digital output
#define SPECTRUM_STROBE 4 //digital output
#define SPECTRUM_RIGHT 1 //analog input
#define SPECTRUM_LEFT 0 //analog input

int spectrum_right[7]; //spectrum analysis values
int spectrum_left[7]; //spectrum analysis values

void setup() {
  // put your setup code here, to run once:
  //Initialize pins for Spectrum Analyzer
  pinMode(SPECTRUM_RESET, OUTPUT);
  pinMode(SPECTRUM_STROBE, OUTPUT);
  digitalWrite(SPECTRUM_STROBE, HIGH);
  digitalWrite(SPECTRUM_RESET, HIGH);

  //Initialize spectrum analyzer
  digitalWrite(SPECTRUM_STROBE, LOW);
  delay(1);
  digitalWrite(SPECTRUM_RESET, HIGH);
  delay(1);
  digitalWrite(SPECTRUM_STROBE, HIGH);
  delay(1);
  digitalWrite(SPECTRUM_STROBE, LOW);
  delay(1);
  digitalWrite(SPECTRUM_RESET, LOW);
  delay(5);

  Serial.begin(57600);

}

void loop() {
  // put your main code here, to run repeatedly:
  readSpectrum();
  for(int i=0;i<7;i++)
  {
    
    Serial.print(i);
    Serial.print(',');
    Serial.println((int)((spectrum_right[i]+spectrum_left[i])/2));
  }
  delayMicroseconds(250);

}

void readSpectrum()
{
  // Band 0 = Lowest Frequencies.
  byte Band;
  for (Band = 0; Band < 7; Band++)
  {
    spectrum_right[Band] = (analogRead(SPECTRUM_LEFT) + analogRead(SPECTRUM_LEFT) ) >> 1; //Read twice and take the average by dividing by 2
    spectrum_left[Band] = (analogRead(SPECTRUM_RIGHT) + analogRead(SPECTRUM_RIGHT) ) >> 1; //Read twice and take the average by dividing by 2
    digitalWrite(SPECTRUM_STROBE, HIGH);
    digitalWrite(SPECTRUM_STROBE, LOW);
  }
}