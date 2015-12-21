#include <Adafruit_NeoPixel.h>
#include "Sparkfun_Spectrum_shield.h"

#define RMS_SAMPLES 1000

#define CALIB_LED 13 //used to determine correct input volume

#define LED_DAT 6 //output pin for the LED strand
#define LED_NUM 200 //number of LEDs in the strand

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LED_DAT, NEO_RGB + NEO_KHZ800);
Sparkfun_Spectrum_Shield spectrum = Sparkfun_Spectrum_Shield();

uint8_t led_buffer[200][3];
unsigned long long runningsum[7];
int thresh[7];
int inc = 0;

void setup() {
  //Initialize all LEDs to 'off'
  strip.begin();
  strip.setBrightness(100);
  strip.show();

  //Initialize spectrum  shield
  spectrum.Init();

  pinMode(CALIB_LED, OUTPUT);

  ClearBuffer();

  for (int i = 0; i < 7; i++)
  {
    thresh[i] = 150;
    runningsum[i] = 0;
  }

  Serial.begin(9600);
}

void loop() {
  FadeRGB();
  spectrum.Update();

  if (spectrum.average[0] == 1023 || spectrum.average[1] == 1023 || spectrum.average[2] == 1023 ||
      spectrum.average[3] == 1023 || spectrum.average[4] == 1023 || spectrum.average[5] == 1023 ||
      spectrum.average[6] == 1023)
  {
    digitalWrite(CALIB_LED, HIGH);
  }
  else
  {
    digitalWrite(CALIB_LED, LOW);
  }

  //Serial.println(spectrum.average[2]);

  for (int i = 0; i < 7; i++)
  {
    runningsum[i] += sq((unsigned long)spectrum.average[i]);
  }

  //Serial.println(runningsum[2]);

  if (inc == RMS_SAMPLES)
  {
    Serial.println("THRESHOLDS");
    for (int i = 0; i < 7; i++)
    {
      thresh[i] = sqrt((unsigned long)(runningsum[i] / RMS_SAMPLES));
      Serial.println(thresh[i]);
      runningsum[i] = 0;
    }
    Serial.println();
    inc = 0;
  }

  for (int i = 0; i < 7; i++)
  {
    if (spectrum.average[i] > thresh[i] + 60)
    {
      BandEvent(i, spectrum.average[i] - thresh[i]);
    }
  }

  WriteBuffer();
  inc++;
}

void BandEvent(int band, int ot)
{

  if (ot > 255)
  {
    ot = 255;
  }
  switch (band)
  {
    case (0):
      for (int i = 0; i < 200; i++)
      {
        led_buffer[i][0] = ot;
      }
      return;
    case (1):
      led_buffer[random(0, 40)][2] = ot;
      return;
    case (2):
      led_buffer[random(40, 80)][2] = ot;
      return;
    case (3):
      led_buffer[random(80, 120)][2] = ot;
      return;
    case (4):
      led_buffer[random(120, 160)][2] = ot;
      return;
    case (5):
      led_buffer[random(160, 200)][2] = ot;
      return;
    case (6):
      for (int i = 160; i < 200; i++)
      {
        led_buffer[i][1] = ot;
      }
      return;
  }
}

void FadeRGB()
{
  for (int i = 0; i < LED_NUM; i++)
  {
    int temp;

    temp = led_buffer[i][0] == 0 ? 0 : led_buffer[i][0] - 3;
    led_buffer[i][0] = temp < 0 ? 0 : temp;

    temp = led_buffer[i][1] == 0 ? 0 : led_buffer[i][1] - 5;
    led_buffer[i][1] = temp < 0 ? 0 : temp;

    temp = led_buffer[i][2] == 0 ? 0 : led_buffer[i][2] - 5;
    led_buffer[i][2] = temp < 0 ? 0 : temp;
  }
}

void ClearBuffer()
{
  for (int i = 0; i < LED_NUM; i++)
  {
    led_buffer[i][0] = 0;
    led_buffer[i][1] = 0;
    led_buffer[i][2] = 0;
  }
}

void WriteBuffer()
{
  for (int i = 0; i < LED_NUM; i++)
  {
    strip.setPixelColor(i, strip.Color(led_buffer[i][0], led_buffer[i][1], led_buffer[i][2]));
  }
  strip.show();
}
