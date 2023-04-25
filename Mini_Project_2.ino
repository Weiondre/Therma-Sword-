#include <Adafruit_NeoPixel.h>

// amt  pin //
Adafruit_NeoPixel strip(60, 4);

//declaring the color of the strip
uint32_t red = strip.Color(255, 69, 0);
uint32_t green = strip.Color(4, 256, 84);
uint32_t purple = strip.Color(166, 1, 255);

int state = 1;

boolean currentState = false;
boolean prevState1 = false;


int buttonPin = 5;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);

  strip.begin();
}

void loop() {
  // functions

  //update prev reading
  prevState1 = currentState;
  // currentState = debounce(buttonPin, prevState1);
  //  loopingLight (green, 555);
  //  bouncingLight (purple, 444);
  //   threeColorChange(red, green, purple, 222);
  // ranbowTimer();
  //Strobe ();

  switch (state) {
    case 1:
      loopingLight (green, 555);
      break;

    case 2:
      bouncingLight (purple, 444);
      break;

    case 3:
      threeColorChange(red, green, purple, 222);
      break;

    case 4:
      rainbowTimer();
      break;

    case 5:
      Strobe(4, 50, 50);
      break;

  }




  // update reading
  currentState = debounce(buttonPin, prevState1);


  // toggle if statement
  if (currentState == true && prevState1 == false) {
    state++;
    if (state > 5) {
      state = 1;
    }
  }

}

void loopingLight(uint32_t aColor, int aTime) {

  static int index = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  if (currentTime - startTime >= aTime) {

    //turn on the first pixel
    strip.setPixelColor(index, aColor);
    strip.show();

    //changes position
    index++;
    strip.clear();

    //starts new loop(timer)
    startTime = millis();

  }
  //send the light back
  if (index >= strip.numPixels()) {
    index = 0;
  }
}

void bouncingLight(uint32_t aColor, int aTime) {

  static int index = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  //boolean to move up strand
  static boolean isGoingUp = true;

  if (isGoingUp == true) {

    if (currentTime - startTime >= aTime) {
      index ++;
      strip.clear();
      startTime = millis();

    }
  }

  //send the light backward
  if (isGoingUp == false) {
    if (currentTime - startTime >= aTime) {
      index --;
      strip.clear();
      startTime = millis();
    }
  }

  strip.setPixelColor(index, aColor);
  strip.show();

  if (index >= strip.numPixels()) {
    isGoingUp = false;
  }

  if (index == 0) {
    isGoingUp = true;
  }
}

void threeColorChange(uint32_t color1, uint32_t color2, uint32_t color3, int aTime) {
  static int state = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  if (state == 0) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if (i % 3 == 0) {
        strip.setPixelColor(i, color1);
      }
      else  if (i % 3 == 1)
        strip.setPixelColor(i, color2);

      else   if (i % 3 == 2)
        strip.setPixelColor(i, color3);

    }
    strip.show();
  }

  if (state == 1) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if (i % 3 == 0) {
        strip.setPixelColor(i, color3);
      }
      else if (i % 3 == 1)
        strip.setPixelColor(i, color1);
      else if (i % 3 == 2)
        strip.setPixelColor(i, color2);
    }

    strip.show();
  }

  if (state == 2) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if (i % 3 == 0) {
        strip.setPixelColor(i, color2);
      }
      else  if (i % 3 == 1)
        strip.setPixelColor(i, color3);
      else   if (i % 3 == 2)
        strip.setPixelColor(i, color1);

    }
    strip.show();
  }

  if (currentTime - startTime >= aTime) {
    state ++;
    if (state > 2) {
      state = 0;
    }
    startTime = millis();
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void rainbowTimer() { // modified from Adafruit example to make it a state machine
  static unsigned long startTime = millis();
  int interval = 20;
  static uint16_t j = 0;


  if (millis() - startTime >= interval){
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    j++;
    if (j >= 256) j = 0;
    startTime = millis(); // time for next change to the display
  }
}


void Strobe(int StrobeCount, int FlashDelay, int EndPause) {

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 128, 0, 128);

  }
  strip.show();
  delay(FlashDelay);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0, 0, 0);

  }
  strip.show();
  delay(FlashDelay);
}


boolean debounce(int aButtonPin, boolean aPrevState) {
  boolean currentState = digitalRead(aButtonPin);

  if (currentState == HIGH && aPrevState == LOW) {
    delay(20);
  }

  return currentState;
}
