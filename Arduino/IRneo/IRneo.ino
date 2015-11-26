
#include <IRremote.h>
#include <Adafruit_NeoPixel.h>


int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);

decode_results results;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, 6, NEO_GRB + NEO_KHZ800);


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
    
    switch(results.value)
    {
      case 0xFFA25D:
        colorWipe(strip.Color(0, 0, 0), 5); // Off
      break;
      
      case 0xFF30CF: // 1
        colorWipe(strip.Color(255, 0, 0), 50); // Red
      break;
      
      case 0xFF18E7: // 2
        colorWipe(strip.Color(0, 255, 0), 50); // Green
      break;
      
      case 0xFF7A85: // 3
        colorWipe(strip.Color(0, 0, 255), 50); // Blue
      break;
      
      case 0xFF10EF: // 4
        theaterChase(strip.Color(127, 127, 127), 50); // White
      break;
      
      case 0xFF38C7: // 5
        theaterChase(strip.Color(0, 0, 127), 50); // Blue
      break;
      
      case 0xFF5AA5: // 6
        rainbow(20); // Rainbow
      break;
      
      case 0xFF42BD: // 7
        rainbowCycle(20); // Rainbow Cycle
      break;
      
      case 0xFF4AB5: // 8
        rainbowCycle(50); // Rainbow Cycle
      break;
      
       default:
      break; 
      
    }
  }
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
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

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
