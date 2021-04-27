#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Digital IO pin connected to the button. This will be driven with a
// pull-up resistor so the switch pulls the pin to ground momentarily.
// On a high -> low transition the button press logic will execute.
#define BUTTON_PIN   4
#define PIXEL_PIN    1  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 4  // Number of NeoPixels
#define MAX_BRIGHTNESS 160
#define COLOR 5000
#define LEVELS 4


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

boolean oldState            = LOW;
int     mode                = 0;    // Currently-active animation mode, 0-9
int     current_brightness  = 0;
int     old_brightness      = 0;
int     i,j;
uint16_t rainbow_step       = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  // rainbowCycle(2);
}


void loop() {
  // Get current button state.
  boolean newState = digitalRead(BUTTON_PIN);

 if((newState == LOW) && (oldState == HIGH)) {
    // Short delay to debounce button.
    delay(100);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if(newState == LOW) {
      mode++;
      if(mode > LEVELS+1){
        mode = 0;
      }

      if(mode > 0 && mode <= LEVELS){
        old_brightness = current_brightness;
        current_brightness += (int)MAX_BRIGHTNESS/(int)LEVELS;
        for(i=old_brightness;i<=current_brightness;i++){
          for(j=0;j<PIXEL_COUNT;j++){
            strip.setPixelColor(j, strip.ColorHSV(COLOR,255,i));
            strip.show();
            delay(1);
          }
        }
      }else if(mode == LEVELS+1){      
        
        if(rainbow_step != 255){
          rainbow_step++;
        }else{
          rainbow_step=0;
        }
       
        for(i=0; i< strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + rainbow_step) & 255));
        }
        strip.show();
        delay(20);


      }else{
        for(i=current_brightness;i>=0;i--){
          for(j=0;j<PIXEL_COUNT;j++){
            strip.setPixelColor(j, strip.ColorHSV(COLOR,255,i));
            strip.show();
            delay(1);
          }
        }
        mode = 0;
        current_brightness = 0;

      }
    }
 }

  if(mode == LEVELS+1){      
        
        if(rainbow_step != 255){
          rainbow_step++;
        }else{
          rainbow_step=0;
        }
       
        for(i=0; i< strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + rainbow_step) & 255));
        }
        strip.show();
        delay(20);
  }
  // Set the last-read button state to the old state.
  oldState = newState;
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
