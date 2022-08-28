#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Keypad.h>
#include <EEPROM.h>

/* Constructor */
U8G2_SSD1309_128X64_NONAME0_2_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);


#define sb_solid_width 33
#define sb_solid_height 26
static char sb_solid_bits[] = {
  0xFC, 0x3F, 0xFE, 0x7F, 0x00, 0xFE, 0x7F, 0xFE, 0xFF, 0x00, 0xFF, 0xFF, 
  0xFE, 0xFF, 0x01, 0xFF, 0xFF, 0xFE, 0xFF, 0x01, 0xFF, 0xFF, 0xFE, 0xFF, 
  0x01, 0xFF, 0xFF, 0xFE, 0xFF, 0x01, 0xFF, 0xFE, 0xFE, 0xFD, 0x01, 0xFF, 
  0xFE, 0xFE, 0xFD, 0x01, 0xFF, 0x00, 0xFE, 0xFD, 0x01, 0xFF, 0x3F, 0xFE, 
  0xFD, 0x01, 0xFF, 0x7F, 0xFE, 0xFD, 0x01, 0xFF, 0xFF, 0xFE, 0xFF, 0x01, 
  0xFF, 0xFF, 0xFE, 0xFF, 0x00, 0xFF, 0xFF, 0xFE, 0x3F, 0x00, 0xFE, 0xFF, 
  0xFE, 0xFF, 0x00, 0xFC, 0xFF, 0xFE, 0xFD, 0x01, 0x00, 0xFE, 0xFE, 0xFD, 
  0x01, 0xFF, 0xFE, 0xFE, 0xFD, 0x01, 0xFF, 0xFE, 0xFE, 0xFD, 0x01, 0xFF, 
  0xFE, 0xFE, 0xFF, 0x01, 0xFF, 0xFE, 0xFE, 0xFF, 0x01, 0xFF, 0xFF, 0xFE, 
  0xFF, 0x01, 0xFF, 0xFF, 0xFE, 0xFF, 0x01, 0xFF, 0xFF, 0xFE, 0xFF, 0x01, 
  0xFE, 0x7F, 0xFE, 0xFF, 0x00, 0xFC, 0x3F, 0xFE, 0x7F, 0x00, };

bool bootInProgress = true;

const byte rows = 8; //four rows
const byte cols = 5; //three columns
const char keys[rows][cols] = {
  {'!','@','#','$','%'},
  {'D','H','O','B','E'},
  {'<','>','d','e','f'}, 
  {'&','~','a','b','c'}, 
  {'|','^','7','8','9'}, 
  {'*','/','4','5','6'}, 
  {'+','-','1','2','3'}, 
  {'S','R','0','X','='}
};
const byte rowPins[rows] = {A0, 12, 7, 6, 5, 4, 3, 2}; //connect to the row pinouts of the keypad
const byte colPins[cols] = {A1, A2, A3, A4, A5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

// Backwards:
const bool binaryMap[16][4] = {
  {false, false, false, false}, // 0000
  {true,  false, false, false}, // 1000
  {false, true,  false, false}, // 0100
  {true,  true,  false, false}, // 1100
  {false, false, true,  false}, // 0010
  {true,  false, true,  false}, // 1010
  {false, true,  true,  false}, // 0110
  {true,  true,  true,  false}, // 1110
  {false, false, false, true }, // 0001
  {true,  false, false, true }, // 1001
  {false, true,  false, true }, // 0101
  {true,  true,  false, true }, // 1101
  {false, false, true,  true }, // 0011
  {true,  false, true,  true }, // 1011
  {false, true,  true,  true }, // 0111
  {true,  true,  true,  true }  // 1111
};



enum menu {
  MAIN_MENU, // Default softkey menu
  BASE, // Select the numerical base system to use
  WORD_SIZE, // Change the register word size
  INFO, // System info screen
  STORE_MENU, // 16 value heap storage display
  STACK_MENU // 16 value stack storage display
};

menu currentSoftkeyMenu = MAIN_MENU;



int base = 16;

enum mode {
  BOOT_SCREEN,
  REGISTER_DISPLAY,
  INFO_SCREEN,
  STORE_SCREEN,
  STACK_SCREEN
};

mode currentMode = BOOT_SCREEN;

// Registers
int Z = 0;
int Y = 0;
int X = 0;
int invisibleSwap = 0;


byte lastOperationStatus = 0%00000010; // All flags 0, Zero is 1.





bool storageModeWaitingOnInput = false;
int storageOperation = 0; // 0 = read, 1 = write


int stackPointer = 0;


int getBase() {
  return base;
}

void setBase(int baseToSet) {
  if (
    (baseToSet == 2) ||
    (baseToSet == 8) ||
    (baseToSet == 10) ||
    (baseToSet == 16)
  ) {
    base = baseToSet;
    EEPROM.put(1022, base);
  }
}





/* u8g2.begin() is required and will sent the setup/init sequence to the display */
void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
  //setBase(16);
  EEPROM.get(1022, base);
  
  EEPROM.get(64, stackPointer);
}


/* draw something on the display with the `firstPage()`/`nextPage()` loop*/
void loop(void) {

  if (Serial.available() > 0) {
    if (Serial.read() == 'S') {
      u8g2.writeBufferXBM(Serial);
    }
  }


  getInput();



 
  u8g2.firstPage();
  do {
    switch (currentMode) {
      case BOOT_SCREEN:
        drawBootScreen();
        break;
      case REGISTER_DISPLAY:
        drawStatus();
        drawValues();
        break;
      case INFO_SCREEN:
        drawInfoScreen();
        break;
      case STORE_SCREEN:
        drawStoreScreen();
        break;
      case STACK_SCREEN:
        drawStackScreen();
        break;
    }
    
    updateSoftkeys();
  } while ( u8g2.nextPage() );
     
 
}
