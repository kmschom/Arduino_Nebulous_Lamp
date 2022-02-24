// A basic everyday NeoPixel strip test program.
// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

boolean flip = false;
boolean topR = false;
boolean topG = false;
boolean topB = true;
float dimmer = 0.0;
int r = 0;
int g = 0;
int b = 255;

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define LED_PIN    10

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 20

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------
void setup() {
  
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); // Set BRIGHTNESS

  pinMode(10, OUTPUT);
  pinMode(5, INPUT);    // Touch sensor controlling Red value
  pinMode(6, INPUT);    // Touch sensor controlling Green value
  pinMode(9, INPUT);    // Touch sensor controlling Blue value
  Serial.begin(9600);
}

void loop() {
  dimmer = analogRead(A2)/1023.0;     // Knob on top of lamp that control brightness of LED strip
  
  if(digitalRead(5) == HIGH) {            // Read if touch sensor is being activated
    if(topR == true and r>=15) r-= 15;    // If value has not reached lower bounds, continue descreasing value
    else topR = false;                    // Otherwise, value has reached lower bound 

    if(topR == false and r<=240) r+=15;   // If value has not reach upper bounds, continue increasing value
    else topR = true;                     // Otherwise, value has reached upper bound 
  }
  if(digitalRead(6) == HIGH) {            // Read if touch sensor is being activated
    if(topG == true and g>=15) g-= 15;    // If value has not reached lower bounds, continue descreasing value
    else topG = false;                    // Otherwise, value has reached lower bound 

    if(topG == false and g<=240) g+=15;   // If value has not reach upper bounds, continue increasing value
    else topG = true;                     // Otherwise, value has reached upper bound 
  }
  if(digitalRead(9) == HIGH) {            // Read if touch sensor is being activated
    if(topB == true and b>=15) b-= 15;    // If value has not reached lower bounds, continue descreasing value
    else topB = false;                    // Otherwise, value has reached lower bound 

    if(topB == false and b<=240) b+=15;   // If value has not reach upper bounds, continue increasing value
    else topB = true;                     // Otherwise, value has reached upper bound 
  }
  
  colorWipe(strip.Color(r*(dimmer),   g*(dimmer),   b*(dimmer)), 1);    // Update color of LED strip
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
