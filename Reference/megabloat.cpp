#include <Adafruit_NeoPixel.h>

#define STATUS_LED 13

#define MID_SR 10

#define LED_DAT 6 //output pin for the LED strand
#define LED_NUM 200 //number of LEDs in the strand

#define SPECTRUM_RESET 5 //digital output
#define SPECTRUM_STROBE 4 //digital output
#define SPECTRUM_RIGHT 1 //analog input
#define SPECTRUM_LEFT 0 //analog input

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_NUM, LED_DAT, NEO_RGB + NEO_KHZ800);

int spectrum_right[7]; //spectrum analysis values
int spectrum_left[7]; //spectrum analysis values

uint8_t led_buffer[200][3];

//the setup() function is automatically run everytime the arduino is powered on
void setup() {

  //Initialize all LEDs to 'off'
  strip.begin();
  strip.setBrightness(100);
  strip.show();

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

  pinMode(STATUS_LED, OUTPUT);

  //set up serial comms (for computer debug)
  Serial.begin(9600);
  //randomSeed(analogRead(0));
  clearBuffer();

}

float bass_lvl = 0;
float mid_lvl1 = 0;
float mid_lvl2 = 0;
float mid_lvl3 = 0;
float mid_lvl4 = 0;
float mid_lvl5 = 0;
int inc = 0;
int high_inc = 0;
long high_runningsum = 0;
int high_thresh = 800;
boolean high_ip = false;
int high_rand_led;
int high_direc;
int high_length;

//the loop() function runs continuously following the setup() funciton
void loop() {
  readSpectrum();
  fadeRed();
  fadeGreen();
  fadeBlue();
  bass_lvl = ((spectrum_right[0] + spectrum_left[0]) / 2 - 100) * (6. / 1023.);
  mid_lvl1 += ((spectrum_right[1] + spectrum_left[1]) / 2. - 100) * (255. / 1023) < 0 ? 0 : ((spectrum_right[1] + spectrum_left[1]) / 2. - 100) * (255. / 1023); //.5*pow(2.7182,.0055*((spectrum_right[1] + spectrum_left[1]) / 2. - 100));
  mid_lvl2 += ((spectrum_right[2] + spectrum_left[2]) / 2. - 100) * (255. / 1023) < 0 ? 0 : ((spectrum_right[2] + spectrum_left[2]) / 2. - 100) * (255. / 1023); //.5*pow(2.7182,.0055*((spectrum_right[2] + spectrum_left[2]) / 2. - 100));
  mid_lvl3 += ((spectrum_right[3] + spectrum_left[3]) / 2. - 100) * (255. / 1023) < 0 ? 0 : ((spectrum_right[3] + spectrum_left[3]) / 2. - 100) * (255. / 1023); //.5*pow(2.7182,.0055*((spectrum_right[3] + spectrum_left[3]) / 2. - 100));
  mid_lvl4 += ((spectrum_right[4] + spectrum_left[4]) / 2. - 100) * (255. / 1023) < 0 ? 0 : ((spectrum_right[4] + spectrum_left[4]) / 2. - 100) * (255. / 1023); //.5*pow(2.7182,.0055*((spectrum_right[4] + spectrum_left[4]) / 2. - 100));
  mid_lvl5 += ((spectrum_right[5] + spectrum_left[5]) / 2. - 100) * (255. / 1023) < 0 ? 0 : ((spectrum_right[5] + spectrum_left[5]) / 2. - 100) * (255. / 1023); //.5*pow(2.7182,.0055*((spectrum_right[5] + spectrum_left[5]) / 2. - 100));

  high_runningsum += (spectrum_right[6] + spectrum_left[6]) / 2;
  if (high_inc == 2000)
  {
    high_thresh = (high_runningsum / 2000) * 2;
    high_runningsum = 0;
    high_inc = 0;
  }
  if ((spectrum_right[6] + spectrum_left[6]) / 2 > high_thresh)
  {
    for (int i = 169; i <= 199; i++)
    {
      led_buffer[i][2] = 75;
    }
  }
  //  if(((spectrum_right[6]+spectrum_left[6])/2>high_thresh) && !high_ip)
  //  {
  //    high_rand_led=random(9,191);
  //    high_direc=random(0,2);
  //    high_length=random(3,11);
  //    high_ip=true;
  //  }
  //  if(high_ip)
  //  {
  //    led_buffer[high_rand_led][2] = 255;
  //    high_rand_led = high_direc==true? high_rand_led+1 : high_rand_led-1;
  //    high_length--;
  //    high_ip = high_length==0 ? false : true;
  //  }

  high_inc++;

  switch ((int)bass_lvl)
  {
    case (5):
      {
        for (int i = 169; i <= 199; i++)
        {
          led_buffer[i][0] = bass_lvl == 5 ? bass_lvl * (255 / 6) : 255;
          //led_buffer[i][0] = 100;
        }
      }
    case (4):
      {
        for (int i = 134; i <= 168; i++)
        {
          led_buffer[i][0] = bass_lvl == 4 ? bass_lvl * (255 / 6) : 255;
          //led_buffer[i][0] = 100;
        }
      }
    case (3):
      {
        for (int i = 94; i <= 133; i++)
        {
          led_buffer[i][0] = bass_lvl == 3 ? bass_lvl * (255 / 6) : 255;
          //led_buffer[i][0] = 100;
        }
      }
    case (2):
      {
        for (int i = 48; i <= 93; i++)
        {
          led_buffer[i][0] = bass_lvl == 2 ? bass_lvl * (255 / 6) : 255;
          //led_buffer[i][0] = 100;
        }
      }
    case (1):
      {
        for (int i = 0; i <= 47; i++)
        {
          led_buffer[i][0] = bass_lvl == 1 ? bass_lvl * (255 / 6) : 255;
          //led_buffer[i][0] = 100;
        }
        break;
      }
  }

  if (inc == MID_SR - 1)
  {
    //Serial.println(inc);
    //Serial.println((uint8_t)(mid_lvl1 / MID_SR));
    //Serial.println((uint8_t)(mid_lvl2 / MID_SR));
    //Serial.println((uint8_t)(mid_lvl3 / MID_SR));
    //Serial.println((uint8_t)(mid_lvl4 / MID_SR));
    //Serial.println((uint8_t)(mid_lvl5 / MID_SR));
    //Serial.println();

    led_buffer[random(0, 47)][1] = (uint8_t)(mid_lvl1 / MID_SR);
    led_buffer[random(48, 93)][1] = (uint8_t)(mid_lvl2 / MID_SR);
    led_buffer[random(94, 133)][1] = (uint8_t)(mid_lvl3 / MID_SR);
    led_buffer[random(134, 168)][1] = (uint8_t)(mid_lvl4 / MID_SR);
    led_buffer[random(169, 200)][1] = (uint8_t)(mid_lvl5 / MID_SR);
    mid_lvl1 = 0;
    mid_lvl2 = 0;
    mid_lvl3 = 0;
    mid_lvl4 = 0;
    mid_lvl5 = 0;
  }

  writeBuffer();
  inc = inc == MID_SR - 1 ? 0 : inc + 1;
  //  //delay(1);
}
void fadeBlue()
{
  for (int i = 0; i < 200; i++)
  {
    int temp;
    temp = led_buffer[i][2] == 0 ? 0 : led_buffer[i][2] - 5;
    led_buffer[i][2] = temp < 0 ? 0 : temp;
  }
}

void fadeRed()
{
  for (int i = 0; i < 200; i++)
  {
    int temp;
    temp = led_buffer[i][0] == 0 ? 0 : led_buffer[i][0] - 7;
    led_buffer[i][0] = temp < 0 ? 0 : temp;
  }
}
void fadeGreen()
{
  for (int i = 0; i < 200; i++)
  {
    int temp;
    temp = led_buffer[i][1] == 0 ? 0 : led_buffer[i][1] - 2;
    led_buffer[i][1] = temp < 0 ? 0 : temp;
  }
}



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorSwipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = strip.numPixels() - 1; i > 0; i--) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorMove(uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    if (i > 0)
    {
      strip.setPixelColor(i - 1, strip.Color(0, 255, 0));
    }
    if (i + 18 < 200)
    {
      strip.setPixelColor(i, strip.Color(1, 250, 0));
      strip.setPixelColor(i + 1, strip.Color(3, 200, 0));
      strip.setPixelColor(i + 2, strip.Color(5, 150, 0));
      strip.setPixelColor(i + 3, strip.Color(10, 100, 0));
      strip.setPixelColor(i + 4, strip.Color(20, 50, 0));
      strip.setPixelColor(i + 5, strip.Color(50, 20, 0));
      strip.setPixelColor(i + 6, strip.Color(100, 10, 0));
      strip.setPixelColor(i + 7, strip.Color(150, 5, 0));
      strip.setPixelColor(i + 8, strip.Color(200, 3, 0));
      strip.setPixelColor(i + 9, strip.Color(250, 1, 0));
      strip.setPixelColor(i + 18, strip.Color(1, 250, 0));
      strip.setPixelColor(i + 17, strip.Color(3, 200, 0));
      strip.setPixelColor(i + 16, strip.Color(5, 150, 0));
      strip.setPixelColor(i + 15, strip.Color(10, 100, 0));
      strip.setPixelColor(i + 14, strip.Color(20, 50, 0));
      strip.setPixelColor(i + 13, strip.Color(50, 20, 0));
      strip.setPixelColor(i + 12, strip.Color(100, 10, 0));
      strip.setPixelColor(i + 11, strip.Color(150, 5, 0));
      strip.setPixelColor(i + 10, strip.Color(200, 3, 0));
    }
    //else
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void clearBuffer()
{
  for (int i = 0; i < 200; i++)
  {
    led_buffer[i][0] = 0;
    led_buffer[i][1] = 0;
    led_buffer[i][2] = 0;
  }
}

void writeBuffer()
{
  for (int i = 0; i < 200; i++)
  {
    strip.setPixelColor(i, strip.Color(led_buffer[i][0], led_buffer[i][1], led_buffer[i][2]));
  }
  strip.show();
}

void randBuffer()
{
  int rlim = 100;
  int glim = 10;
  int blim = 10;
  int state = 0;

  for (;;)
  {
    switch (state)
    {
      case (0): //red
        rlim--;
        blim++;
        if (rlim == 10)
          state = 1;
        break;
      case (1): //blue
        blim--;
        glim++;
        if (blim == 10)
          state = 2;
        break;
      case (2): //green
        glim--;
        rlim++;
        if (glim == 10)
          state = 0;
        break;
    }

    for (int i = 0; i < 200; i++)
    {
      led_buffer[i][0] = random(0, rlim);
      led_buffer[i][1] = random(0, glim);
      led_buffer[i][2] = random(0, blim);
    }
    writeBuffer();
    delay(50);
  }
}

void idLED()
{
  char temp;
  boolean cont = true;
  int ledposition = 0;
  clearBuffer();
  while (cont)
  {
    temp = Serial.read();
    if (temp == 'f')
    {
      led_buffer[ledposition][2] = 255;
      Serial.print("Set LED: ");
      Serial.println(ledposition);
      ledposition++;
      if (ledposition > 199)
      {
        ledposition = 199;
      }
    }
    else if (temp == 'b')
    {
      led_buffer[ledposition][2] = 0;
      Serial.print("Cleared LED: ");
      Serial.println(ledposition);
      ledposition--;
      if (ledposition < 0)
      {
        ledposition = 0;
      }
    }
    else if (temp == 'x')
    {
      cont == false;
      Serial.println("DONE");
    }
    writeBuffer();
  }
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