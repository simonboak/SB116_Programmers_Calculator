int currentStoragePage = 1;

void storeModeInputHandler(char key) {
  int inputNumber;
  
  if (storageModeWaitingOnInput) {
    if (
    ((key >= 48) && (key <= 57)) ||  // numbers 0 to 9 in ASCII
    ((key >= 97) && (key <= 102))     // characters a to f
    ) {
      storageModeWaitingOnInput = false;

      inputNumber = asciiToInt(key);
      
      switch (storageOperation) {
        case 0: // 0 = read
          EEPROM.get(inputNumber * 2, X);
          currentMode = REGISTER_DISPLAY;
          currentSoftkeyMenu = MAIN_MENU;
          break;
        case 1: // 1 = write
          EEPROM.put(inputNumber * 2, X);
          break;
      }
    }
  } else if (key == 'D') { // Set decimal display
    setBase(10);
  } else if (key == 'H') { // Set hexadecimal display
    setBase(16);
  } else if (key == 'O') { // Set octal display
    setBase(8);
  } else if (key == 'B') { // Set binary display
    setBase(2);
  }
}



void drawStoreScreen() {
  String statusString = "STORAGE  -  PAGE ";
  statusString += currentStoragePage;
  statusString += "/4";
  
  drawString(1, 6, statusString, 0, false);

  // Status bar's horizontal line
  u8g2.setDrawColor(1);
  u8g2.drawHLine(0, 8, 128);

  int y = 21; // Y coord of each line to show
  int storeAddress = (currentStoragePage - 1) * 4;

  for (int i = 0; i < 4; i++) {
    String storageLine;
    storageLine += String(storeAddress + i, HEX);
    storageLine += ": ";

    int storageVal;
    EEPROM.get((storeAddress + i) * 2, storageVal);
    
    switch (base) {
      case 10:
        storageLine += String(storageVal, DEC);
        break;
      case 16:
        storageLine += String(storageVal, HEX);
        break;
      case 8:
        storageLine += String(storageVal, OCT);
        break;
      case 2:
        storageLine += String(storageVal, BIN);
        break;
    }
    
    storageLine.toUpperCase();
    
    drawString(1, y, storageLine, 1, false);

    y += 10;
  }



  // Show input bar
  if (storageModeWaitingOnInput) {
    switch (storageOperation) {
      case 0: // 0 = read
        drawString(1, 63, "READ FROM (0-F): ", 0, false);
        break;
      case 1: // 1 = write
        drawString(1, 63, "WRITE TO (0-F): ", 0, false);
        break;
    }
    
  }
}



void storeModePrevPage() {
  currentStoragePage--;

  if (currentStoragePage < 1) {
     currentStoragePage = 4;
  }
}


void storeModeNextPage() {
  currentStoragePage++;

  if (currentStoragePage > 4) {
     currentStoragePage = 1;
  }
}
