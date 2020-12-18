#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define SIGNAL_PIN 10
#define NUMPIXELS 42  // Number of leds
#define BRIGHTNESS  255 // set max brightness
#define IWAIT   2000 // wait times
#define SWAIT   20  // wait times
#define LWAIT   50  // wait times
#define HWAIT   1500  // wait times

const int switchPin = 4;
const int potPin = 2;
int ledState = LOW; // This is the state of the leds
int switchState = LOW; // This is the state of the switch
int potValue = LOW; // This is the state of the pot
int switchExistingState = 0; // monitors change in the switch
int potExistingState = 10; // monitors change in the pot
int x; // temp variable for calculating percentages
int var_delay; // temp variable for changing delay rate below

int led_addresses[] = {0, 11, 12, 23, 24, 35, 36,
                        1, 10, 13, 22, 25, 34, 37,
                        2, 9, 14, 21, 26, 33, 38,
                        3, 8, 15, 20, 27, 32, 39,
                        4, 7, 16, 19, 28, 31, 40,
                        5, 6, 17, 18, 29, 30, 41};

// Initializes the leds
Adafruit_NeoPixel pixels(NUMPIXELS, SIGNAL_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  pinMode(switchPin, INPUT);
  pinMode(potPin, INPUT);
  pixels.begin();
  pixels.setBrightness(BRIGHTNESS);
  Serial.begin(9600); // for debugging
  Serial.println("Starting up");
  pixels.clear(); // clears the pixels at the begining
}

void loop() {
  
  switchState = digitalRead(switchPin);
  potValue = analogRead(potPin);

    // Light switch turns off
  if (switchState == LOW and switchExistingState == 1){
    turnOff();
  }
  
  // Light switch turns on
  if (switchState == HIGH and switchExistingState == 0){
    turnOn();
  }

  // Checks for pot value and adds color to the lights
  if ((switchState == HIGH) && (switchExistingState == 1)){
    changeLightColour(potValue);
    }
  
  switchExistingState = switchState; // Updates old value for next round
  potExistingState = potValue; //  Updates old value for next round

}

//void turnOn(){
//  Serial.println("Turn on function called");
//  for (int i=0; i< NUMPIXELS; i++){
//    pixels.setPixelColor(i, pixels.Color(0, 0, 0, 255));
//    pixels.show();
//    delay(50);
//    }
//}


void turnOn(){
    for (int i=0; i< sizeof(led_addresses)/sizeof(led_addresses[0]); i=i+1){
      pixels.setPixelColor(led_addresses[i], pixels.Color(0, 0, 0, 255));
      pixels.show();
      delay(50); 
      }
}


void turnOff(){
  for (int i=NUMPIXELS-1; i >= 0; i=i-1){
    pixels.setPixelColor(led_addresses[i], pixels.Color(0, 0, 0, 0));
    pixels.show();
    delay(200); 
    }
}


void changeLightColour(int colourValue) {
  colourValue = map(colourValue, 0, 840, 0, 10);
  potExistingState = map(potExistingState, 0, 840, 0 ,10);
  
//  light swtich is set to on
  if ((colourValue >= 8) && (potExistingState != colourValue)){
    turnOn();
    } else if ((8 > colourValue) && (colourValue >= 4)) {
        setPixelColour(colourValue);
    } else if ((3 > colourValue) && (colourValue >= 1)) {
        setFastPixelColour(colourValue);
    } else {
        rainbowCycle(LWAIT);
    }
  delay(var_delay);
}

void setPixelColour(int colourValue){
  var_delay = 80 * colourValue;
  x = random(10);
  if (x > colourValue){
      pixels.setPixelColor(random(NUMPIXELS), pixels.Color(random(255), random(255), random(255), 0));
      pixels.show(); 
    } else {
        pixels.setPixelColor(random(NUMPIXELS), 0, 0, 0, 255);
      }
  }


void setFastPixelColour(int colourValue){
    var_delay = 50* colourValue;
    pixels.setPixelColor(random(NUMPIXELS), pixels.Color(random(255), random(255), random(255), 0));
    pixels.show(); 
  }


// This makes the rainbow equally distributed throughout
void rainbowCycle(uint16_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    potValue = analogRead(potPin);
    int colourValue = map(potValue, 0, 840, 0, 10);
    if (colourValue < 1){
          for(i=0; i< pixels.numPixels(); i++) {
          pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
        }
      } else{
        break;
        }
    pixels.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
