// Transmitter code
const int NRF_CE_PIN = A4, NRF_CSN_PIN = A5;

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

// Transmitter pin usage
const int LCD_RS_PIN = 3, LCD_EN_PIN = 2, LCD_D4_PIN = 4, LCD_D5_PIN = 5, LCD_D6_PIN = 6, LCD_D7_PIN = 7;
const int SW1_PIN = 8, SW2_PIN = 9, SW3_PIN = 10, SW4_PIN = A3, SW5_PIN = A2;

// LCD library code
#include <LiquidCrystal.h>

// initialize the library with the relevant pins
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

const int NUM_OF_STATES = 17;
char *theStates[] = {
    "0 open window",
    "1 close window",
    "2 yes led",
    "3 no led",
    "4 uae led",
    "5 clear led",
    "6 sound ads",
    "7 script 1",
    "8 script 2",
    "9 script 3",
    "10 script 4",
    "11 script 5",
    "12 script 6",
    "13 script 7",
    "14 hello",
    "15 agreed",
    "16 yamete",
};

void updateLCD()
{

    lcd.clear();
    lcd.print(theStates[data.stateNumber]);
    lcd.setCursor(0, 1); // column, line (from 0)
    lcd.print("not transmitted yet");
}

void countDown()
{
    data.stateNumber = (data.stateNumber > 0) ? (data.stateNumber - 1) : 0;
    updateLCD();
}

void countUp()
{
    if (++data.stateNumber >= NUM_OF_STATES)
    {
        data.stateNumber = NUM_OF_STATES - 1;
    }
    updateLCD();
}

void spare1() {}
void spare2() {}

void rf24SendData()
{

    radio.stopListening(); // go into transmit mode
    // The write() function will block
    // until the message is successfully acknowledged by the receiver
    // or the timeout/retransmit maxima are reached.
    int retval = radio.write(&data, sizeof(data));

    lcd.clear();
    lcd.setCursor(0, 0); // column, line (from 0)
    lcd.print("transmitting");
    lcd.setCursor(14, 0); // column, line (from 0)
    lcd.print(data.stateNumber);

    // Serial.print(F(" ... "));
    // if (retval) {
    Serial.println(F("success"));
    lcd.setCursor(0, 1); // column, line (from 0)
    lcd.print("success");
    // } else {
    //   totalTransmitFailures++;
    //   Serial.print(F("failure, total failures = "));
    //   Serial.println(totalTransmitFailures);

    //   lcd.setCursor(0, 1); // column, line (from 0)
    //   lcd.print("error, total=");
    //   lcd.setCursor(13, 1); // column, line (from 0)
    //   lcd.print(totalTransmitFailures);
    // }
}

class Button
{
    int pinNumber;
    bool previousState;
    void (*buttonFunction)();

public:
    // Constructor
    Button(int pn, void *bf)
    {
        pinNumber = pn;
        buttonFunction = bf;
        previousState = 1;
    }

    // update the button
    void update()
    {
        bool currentState = digitalRead(pinNumber);
        if (currentState == LOW && previousState == HIGH)
        {
            Serial.print("button on pin ");
            Serial.print(pinNumber);
            Serial.println();
            buttonFunction();
        }
        previousState = currentState;
    }
};

const int NUMBUTTONS = 5;
Button theButtons[] = {
    Button(SW1_PIN, countDown),
    Button(SW2_PIN, rf24SendData),
    Button(SW3_PIN, countUp),
    Button(SW4_PIN, spare1),
    Button(SW5_PIN, spare2),
};

void setupRF24()
{

    setupRF24Common();

    // Set us as a transmitter
    radio.openWritingPipe(xmtrAddress);
    radio.openReadingPipe(1, rcvrAddress);

    // radio.printPrettyDetails();
    Serial.println(F("I am a transmitter"));

    data.stateNumber = 0;
}

void setup()
{
    Serial.begin(9600);

    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Radio init ....");

    setupRF24();

    // Initialize the switches
    pinMode(SW1_PIN, INPUT_PULLUP);
    pinMode(SW2_PIN, INPUT_PULLUP);
    pinMode(SW3_PIN, INPUT_PULLUP);
    pinMode(SW4_PIN, INPUT_PULLUP);
    pinMode(SW5_PIN, INPUT_PULLUP);
    lcd.setCursor(0, 1); // column, line (from 0)
    lcd.print("setup() finished");

    updateLCD();
}

void loop()
{
    for (int i = 0; i < NUMBUTTONS; i++)
    {
        theButtons[i].update();
    }
    delay(50); // for testing
}
