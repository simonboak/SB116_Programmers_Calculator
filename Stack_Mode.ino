int currentStackPage = 1;
const int stackBase = 32;
int stackPageCount = 1;

void stackModeInputHandler(char key) {
  if (key == 'D') { // Set decimal display
    setBase(10);
  } else if (key == 'H') { // Set hexadecimal display
    setBase(16);
  } else if (key == 'O') { // Set octal display
    setBase(8);
  } else if (key == 'B') { // Set binary display
    setBase(2);
  }
}


void drawStackScreen() {
  stackPageCount = 1;
  if (stackPointer >= 12) {
    stackPageCount = 4;
  } else if (stackPointer >= 8) {
    stackPageCount = 3;
  } else if (stackPointer >= 4) {
    stackPageCount = 2;
  }

  
  String statusString = "STACK  -  PAGE ";
  statusString += currentStackPage;
  statusString += "/" + String(stackPageCount);
  
  drawString(1, 6, statusString, 0, false);

  // Status bar's horizontal line
  u8g2.setDrawColor(1);
  u8g2.drawHLine(0, 8, 128);

  int y = 21; // Y coord of each line to show
  int stackAddress = ((currentStackPage - 1) * 4) + stackBase;

  

  for (int i = 0; i < 4; i++) {
    String stackLine;

    int stackItem;
    if (currentStackPage == 1) {
      stackItem = stackPointer - i;
    } else {
      //stackItem = (stackPointer - ((stackPageCount - currentStackPage) * 4)) + i;
      stackItem = stackPointer - ((currentStackPage - 1) * 4) - i;
    }

    if (stackItem < 0) {
      break;
    }
    
    stackLine += String(stackItem, HEX);
    stackLine += ": ";


    int stackVal;
    //EEPROM.get((stackAddress + stackBase) * 2, stackVal);
    EEPROM.get((stackItem * 2) + stackBase, stackVal);
    
    switch (base) {
      case 10:
        stackLine += String(stackVal, DEC);
        break;
      case 16:
        stackLine += String(stackVal, HEX);
        break;
      case 8:
        stackLine += String(stackVal, OCT);
        break;
      case 2:
        stackLine += String(stackVal, BIN);
        break;
    }

    stackLine.toUpperCase();
    
    drawString(1, y, stackLine, 1, false);

    y += 10;
  }
}


void stackModePull() {
  if (stackPointer >= 0) {
    currentStackPage = 1;
    EEPROM.get((stackPointer * 2) + stackBase, X);

    //if (stackPointer > 0) {
      stackPointer--;
      EEPROM.put(64, stackPointer);
    //}
  }
}

void stackModePush() {
  if (stackPointer < 15) {
    currentStackPage = 1;
    stackPointer++;
    EEPROM.put((stackPointer * 2) + stackBase, X);
    EEPROM.put(64, stackPointer);
  }
}


void stackModeClear() {
  stackPointer = -1;
  EEPROM.put(64, stackPointer);
  EEPROM.put(stackBase, 0);
  currentStackPage = 1;
  stackPageCount = 1;
}


void stackModePrevPage() {
  currentStackPage--;

  if (currentStackPage < 1) {
     currentStackPage = stackPageCount;
  }
}


void stackModeNextPage() {
  currentStackPage++;

  if (currentStackPage > stackPageCount) {
     currentStackPage = 1;
  }
}
