// For the receiver
const int NRF_CE_PIN = A11, NRF_CSN_PIN = A15;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN); // CE, CSN

const byte addr = 0xC3;
const int RF24_CHANNEL_NUMBER = 70;

// Do not make changes here
const byte xmtrAddress[] = {addr, addr, 0xC7, 0xE6, 0xCC};
const byte rcvrAddress[] = {addr, addr, 0xC7, 0xE6, 0x66};

const int RF24_POWER_LEVEL = RF24_PA_LOW;

// global variables
uint8_t pipeNum;
unsigned int totalTransmitFailures = 0;

struct DataStruct
{
    uint8_t stateNumber;
};
DataStruct data;

void setupRF24Common()
{

    // RF24 setup
    if (!radio.begin())
    {
        Serial.println(F("radio initialization failed"));
        while (1)
            ;
    }
    else
    {
        Serial.println(F("radio successfully initialized"));
    }

    radio.setDataRate(RF24_250KBPS);
    radio.setChannel(RF24_CHANNEL_NUMBER);
    radio.setPALevel(RF24_POWER_LEVEL);
}

// Receiver Code

// Additional libraries for music maker shield
#include <Adafruit_VS1053.h>
// #include <SD.h>

// Servo library
#include <Servo.h>

// Additional libraries for graphics on the Neo Pixel Matrix
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Adafruit_VS1053.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

// NEOPIXELS 
#define PIN1 20
#define PIN2 21
#define PIN3 19
#define NUMPIXELS 64

Adafruit_NeoPixel neopixel1 = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neopixel2 = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neopixel3 = Adafruit_NeoPixel(NUMPIXELS, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neopixels[] = {neopixel1, neopixel2, neopixel3};

// Neopixel Colors
uint32_t colors[] = {
  neopixel1.Color(0, 255, 0),     // 0 : green
  neopixel1.Color(255, 0, 0),     // 1 : red,
  neopixel1.Color(255, 255, 0),   // 2 : yellow
  neopixel1.Color(255, 255, 255), // 3 : white
  neopixel1.Color(10, 10, 10),    // 4 : black
  neopixel1.Color(128, 128, 128), // 5 : grey
};

// NEOPIXELS 

// MMS 
#define SHIELD_RESET -1 // VS1053 reset pin (unused!)
#define SHIELD_CS 7     // VS1053 chip select pin (output)
#define SHIELD_DCS 6    // VS1053 Data/command select pin (output)
#define CARDCS 4        // Card chip select pin
#define DREQ 3          // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
// MMS 

// SERVO MOTOR 
#define ServoPIN 16

Servo window;

const int OPEN_WINDOW = -10;
const int CLOSE_WINDOW = 45;

// SERVO MOTOR 

void setup()
{
    Serial.begin(9600);

    // Set up all the attached hardware
    setupMusicMakerShield();
    setupServoMotors();
    setupNeoPixels();

    setupRF24();

    // Brief flash to show we're done with setup()
    flashNeoPixels();
}

void setupRF24()
{
    setupRF24Common();

    // Set us as a receiver
    radio.openWritingPipe(rcvrAddress);
    radio.openReadingPipe(1, xmtrAddress);

    // radio.printPrettyDetails();
    Serial.println(F("I am a receiver"));
}

void setupMusicMakerShield() {
    if (!musicPlayer.begin())
    { // initialise the music player
        Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
        while (1)
            ;
    }
    Serial.println(F("VS1053 found"));

    if (!SD.begin(CARDCS))
    {
        Serial.println(F("SD card failed or not present"));
        while (1)
            ; // don't do anything more
    }

    // Set volume for left, right channels. lower numbers == louder volume!
    musicPlayer.setVolume(20, 20);

    // audio playing
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT); // DREQ int
}

void setupServoMotors() {
    window.attach(ServoPIN);
    window.write(CLOSE_WINDOW);
}

void setupNeoPixels() {
 for (int k = 0; k < 3; k++) {
  neopixels[k].begin();
  neopixels[k].clear();
  neopixels[k].setBrightness(100); 
 }
}

void window_open() {
  for (int i = CLOSE_WINDOW; i >= OPEN_WINDOW; i-= 2) {
    window.write(i);
    delay(100);
  }
}

void window_close() {
  for (int i = OPEN_WINDOW; i <= CLOSE_WINDOW; i+= 2) {
    window.write(i);
    delay(100);
  }
}

void flashNeoPixels()
{
  for (int k = 0; k < 3; k++) {
    for (int j = 0; j < 64; j++)
       neopixels[k].setPixelColor(j, colors[0]);
    neopixels[k].show();
  }
  delay(50);
  neopixel_clear();
}

void dollar(int color) {
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 4; j++)
        neopixels[k].setPixelColor(i * 24 + j + 2 , colors[color]);
    for (int i = 0; i < 2; i++) {
      neopixels[k].setPixelColor((i + 1) * 8 + 1 , colors[color]);
      neopixels[k].setPixelColor((i + 5) * 8 - 2 , colors[color]);
    }
    neopixels[k].setPixelColor(14, colors[color]);
    neopixels[k].setPixelColor(41, colors[color]);

    for (int c = 1; c < 4; c++) {
      for (int i = 0; i < 8; i++) {
        neopixels[k].setPixelColor(i * 8, colors[c]);
        neopixels[k].show();
        delay(100);
      }
      for (int i = 0; i < 7; i++) {
        neopixels[k].setPixelColor(i + 57, colors[c]);
        neopixels[k].show();
        delay(100);
      }
      for (int i = 0; i < 7; i++) {
        neopixels[k].setPixelColor((7 - i) * 8 - 1 , colors[c]);
        neopixels[k].show();
        delay(100);
      }  
    }
    delay(1500);
    neopixels[k].clear();
    neopixels[k].show();
  }
}

void led_show() {
  for (int k = 0; k < 3; k++)
    neopixels[k].show();
}

void uae_flag() {
  for (int k = 0; k < 3; k++) {
    // red
    for (int i = 0; i < 8; i++) {
      neopixels[k].setPixelColor(i * 8, colors[1]);
      neopixels[k].setPixelColor(i * 8 + 1, colors[1]);
    }

    // green
    for (int j = 0; j < 3; j++)
      for (int i = 0; i < 6; i++) 
      neopixels[k].setPixelColor(j * 8 + i + 2, colors[0]);

    // white
    for (int j = 3; j < 5; j++)
      for (int i = 0; i < 6; i++)
        neopixels[k].setPixelColor(j * 8 + i + 2, colors[3]);

    // red
    for (int j = 5; j < 8; j++)
      for (int i = 0; i < 6; i++)
      neopixels[k].setPixelColor(j * 8 + i + 2, colors[4]);

    neopixels[k].show();
  }
}

void confused_face(int color) {
  // LEFT & RIGHT EYE
  for (int k = 0; k < 2; k++) {
    for (int i = 0; i < 4; i++) {
      neopixels[k].setPixelColor(i + 2, colors[color]);
      neopixels[k].setPixelColor(i + 58, colors[color]);
    }
    for (int i = 0; i < 2; i++) {
      neopixels[k].setPixelColor(i * 5 + 9, colors[color]);
      neopixels[k].setPixelColor(i * 5 + 49, colors[color]);
    }
    for (int i = 0; i < 4; i++) {
      neopixels[k].setPixelColor(i * 8 + 16, colors[color]);
      neopixels[k].setPixelColor(i * 8 + 23, colors[color]);
    }
  }
  // MOUTH
  for (int i = 0; i < 4; i++) {
    neopixels[2].setPixelColor(i * 7 + 35, colors[color]);
    neopixels[2].setPixelColor(i * 9 + 36, colors[color]);
  }
}

void winky_face(int color) {
  // LEFT EYE
  neopixels[0].setPixelColor(2, colors[color]);
  neopixels[0].setPixelColor(3, colors[color]);
  neopixels[0].setPixelColor(9, colors[color]);
  neopixels[0].setPixelColor(16, colors[color]);
  neopixels[0].setPixelColor(24, colors[color]);
  for (int i = 0; i < 3; i++) {
    neopixels[0].setPixelColor(i + 19, colors[color]);
    neopixels[0].setPixelColor(i + 59, colors[color]);
  }
  for (int i = 0; i < 4; i++) {
    neopixels[0].setPixelColor(i * 8 + 26, colors[color]);
    neopixels[0].setPixelColor(i * 8 + 30, colors[color]);
  }

  // RIGHT EYE
  for (int i = 0; i < 4; i++)
    neopixels[1].setPixelColor(i + 11, colors[color]);
  neopixels[1].setPixelColor(23, colors[color]);
  for (int i = 0; i < 4; i++)
    neopixels[1].setPixelColor(i + 26, colors[color]);
  neopixels[1].setPixelColor(33, colors[color]);
  neopixels[1].setPixelColor(38, colors[color]);

  // MOUTH
  for (int i = 0; i < 4; i++) {
    neopixels[2].setPixelColor(i * 9 + 32, colors[color]);
    neopixels[2].setPixelColor(i * 7 + 39, colors[color]);
  }
}

void tongue_face(int eyes, int mouth) {
  // EYES
  for (int i = 0; i < 4; i++) {
    neopixels[0].setPixelColor(i * 9 + 4, colors[eyes]);
    neopixels[0].setPixelColor(i * 7 + 39, colors[eyes]);
    neopixels[1].setPixelColor(i * 9 + 32, colors[eyes]);
    neopixels[1].setPixelColor(i * 7 + 3, colors[eyes]);
  }
  // MOUTH
  for (int i = 0; i < 4; i++) {
    neopixels[2].setPixelColor(i * 8 + 1, colors[mouth]);
    neopixels[2].setPixelColor(i * 8 + 6, colors[mouth]);
  }
  for (int i = 0; i < 2; i++) {
    neopixels[2].setPixelColor(i * 3 + 34, colors[mouth]);
    neopixels[2].setPixelColor(i + 43, colors[mouth]);
  }
}

void heart_face(int *color) {
  for (int k = 0; k < 3; k ++) {
    for (int i = 9; i < 14; i++) 
      if (i != 11)
        neopixels[k].setPixelColor(i, colors[color[0]]);
    for (int i = 16; i < 46; i++) 
      if (i != 40 && i != 23 && i != 31 && i != 39)
        neopixels[k].setPixelColor(i, colors[color[0]]);
    for (int i = 50; i < 53; i++)
      neopixels[k].setPixelColor(i, colors[color[0]]);
    neopixels[k].setPixelColor(59, colors[color[0]]);
  }
}

void unamused_face(int eye, int eyebrow, int mouth) {
  for (int k = 0; k < 2; k++) {
    // EYEBROWS
    for (int i = 2; i < 6; i++) {
      neopixels[k].setPixelColor(i, colors[eyebrow]);
    }
    neopixels[k].setPixelColor(9, colors[eyebrow]);
    neopixels[k].setPixelColor(14, colors[eyebrow]);
    // EYES
    for (int i = 0; i < 3; i++) {
      neopixels[k].setPixelColor(i + 27, colors[eye]);
      neopixels[k].setPixelColor(i + 37, colors[eye]);
      neopixels[k].setPixelColor(i + 45, colors[eye]);
      neopixels[k].setPixelColor(i + 53, colors[eye]);
    }
    neopixels[k].setPixelColor(34, colors[eye]);
  }
  // MOUTH
  for (int i = 0; i < 4; i++) {
    neopixels[2].setPixelColor(i * 7 + 35, colors[mouth]);
    neopixels[2].setPixelColor(i * 9 + 36, colors[mouth]);
  }
}

void neopixel_clear() {
  for (int k = 0; k < 3; k++) {
    neopixels[k].clear();
    neopixels[k].show();
  }
}

void neopixel_clear_pretty() {
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < 64; i++) {
      neopixels[k].setPixelColor(i, 0);
      neopixels[k].show();
      delay(10);
    }
  }
}

void state_zero() {
  window_open();
}

void state_one() {
  window_close();
}

void state_two() {
  //
}

void state_three() {
  //
}

void state_four() {
  uae_flag();
}

void state_five() {
  neopixel_clear_pretty();
}

void state_six() {
  musicPlayer.startPlayingFile("/ads.mp3");
}

void state_seven() {
  neopixel_clear();
  confused_face(2);
  led_show();
  musicPlayer.startPlayingFile("/one.mp3");
}

void state_eight() {
  neopixel_clear();
  winky_face(2);
  led_show();
  musicPlayer.startPlayingFile("/two.mp3");
}

void state_nine() {
  neopixel_clear();
  tongue_face(2, 1);
  led_show();
  musicPlayer.startPlayingFile("/three.mp3");
}

void state_ten() {
  int color[] = {0, 1, 2};
  neopixel_clear();
  heart_face(color);
  led_show();
  musicPlayer.startPlayingFile("/four.mp3");
}

void state_eleven() {
  neopixel_clear();
  unamused_face(2, 2, 5);
  led_show();
  musicPlayer.startPlayingFile("/five.mp3");
}

void state_twelve() {
  neopixel_clear();
  unamused_face(2, 2, 5);
  led_show();
  musicPlayer.startPlayingFile("/six.mp3");
}

void state_thirteen() {
  neopixel_clear();
  tongue_face(2, 1);
  led_show();
  musicPlayer.startPlayingFile("/seven.mp3");
}

void state_fourteen() {
  musicPlayer.startPlayingFile("/hello.mp3");
}

void state_fiveteen() {
  musicPlayer.startPlayingFile("/agree.mp3");
}

void state_sixteen() {
  musicPlayer.startPlayingFile("/yamete.mp3");
}
void loop()
{
  // If there is data, read it,
  // and do the needfull
  // Become a receiver
  radio.startListening();
  if (radio.available(&pipeNum))
  {
      radio.read(&data, sizeof(data));
      Serial.print(F("message received Data = "));
      Serial.print(data.stateNumber);
      Serial.println();

      switch (data.stateNumber)
      {
      case 0:
        state_zero();
        break;
      case 1:
        state_one();
        break;
      case 2:
        state_two();
        break;
      case 3:
        state_three();
        break;
      case 4:
        state_four();
        break;
      case 5:
        state_five();
        break;
      case 6:
        state_six();
        break;
      case 7:
        state_seven();
        break;
      case 8:
        state_eight();
        break;
      case 9:
        state_nine();
        break;
      case 10:
        state_ten();
        break;
      case 11:
        state_eleven();
        break;
      case 12:
        state_twelve();
        break;
      case 13:
        state_thirteen();
        break;
      case 14:
        state_fourteen();
        break;
      case 15:
        state_fiveteen();
        break;
      case 16:
        state_sixteen();
        break;
      default:
          Serial.println(F("Invalid option"));
      }
  }
} // end of loop()
