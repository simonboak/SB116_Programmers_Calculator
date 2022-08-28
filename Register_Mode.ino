void registerModeInputHandler(char key) {
  base = getBase();
  
  if (
    ((key >= 48) && (key <= 57)) ||  // numbers 0 to 9 in ASCII
    ((key >= 97) && (key <= 102))     // characters a to f
    ) {
      inputDigit(key);
  } else if (key == '=') {
    Z = Y;
    Y = X;
    X = 0;
    lastOperationStatus = SREG;
  } else if (key == 'D') { // Set decimal display
    setBase(10);
  } else if (key == 'H') { // Set hexadecimal display
    setBase(16);
  } else if (key == 'O') { // Set octal display
    setBase(8);
  } else if (key == 'B') { // Set binary display
    setBase(2);
  } else if (key == 'S') { // Swap the X and Y registers
    invisibleSwap = X;
    X = Y;
    Y = invisibleSwap;
    lastOperationStatus = SREG;
  } else if (key == 'R') { // Roll the registers downwards
    invisibleSwap = X;
    X = Y;
    Y = Z;
    Z = invisibleSwap;
    lastOperationStatus = SREG;
  } else if (key == 'X') { // Invert the sign of the X register
    X = X * -1;
    lastOperationStatus = SREG;
  } else if ( // These transform the value of the X register (i.e. not a calculation)
    (key == '~') ||
    (key == '<') ||
    (key == '>')
  ) {
    inputTransform(key);
  } else if ( // Perform operation on the X and Y registers.
    (key == '+') ||
    (key == '-') ||
    (key == '*') ||
    (key == '/') ||
    (key == '&') ||
    (key == '|') ||
    (key == '^')
  ) {
      inputOperator(key);
  }
}


void inputDigit(int inputNumber) {
  inputNumber = asciiToInt(inputNumber);

  
  // Allow only the correct input digits for the current base
  if (
    ((base == 2) && (inputNumber > 1)) ||
    ((base == 8) && (inputNumber > 7)) ||
    ((base == 10) && (inputNumber > 9))
  ) {
      return;
  }

  
  int shiftRegisterBy = 4;
  if (base == 8) { // if octal
    shiftRegisterBy = 3;
  } else if (base == 2) { // if binary
    shiftRegisterBy = 1;
  }

  if (base == 10) {
    if (X >= 0) {
      // X is currently postive. Only append the digit if we don't roll over 32,767 into a negative number
      if ( ((X * 10) + inputNumber) >= 0 ) {
        X = X * 10;
        X += inputNumber;
      }
    } else {
      // X is currently negative. Only append the digit if we don't roll up into a posative number
      if ( ((X * 10) + inputNumber) < 0 ) {
        X = X * 10;
        X += inputNumber;
      }
    }
    //X = X * 10;
  } else {
    X = X << shiftRegisterBy;
    X += inputNumber;
  }

  
  
}


void inputOperator(char inputOperator) {
  switch (inputOperator) {
    case '+':
      X = Y + X;
      break;
    case '-':
      X = Y - X;
      break;
    case '*':
      X = Y * X;
      break;
    case '/':
      X = Y / X;
      break;
    case '&':
      X = Y & X;
      break;
    case '|':
      X = Y | X;
      break;
    case '^':
      X = Y ^ X;
      break;
  }

  lastOperationStatus = SREG;
  Y = Z;
}


void inputTransform(char inputTransformer) {
  switch (inputTransformer) {
    case '>':
      X = X >> 1;
      break;
    case '<':
      X = X << 1;
      break;
    case '~':
      X = ~X;
      break;
  }

  lastOperationStatus = SREG;
}










void drawValues() {  
  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.setCursor(1, 22);
  u8g2.print("Z:");
  printRegister(Z, 70, 22); // was 30

  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.setCursor(1, 36);
  u8g2.print("Y:");
  printRegister(Y, 70, 36);

  u8g2.setFont(u8g2_font_9x15_mf);
  u8g2.setCursor(1, 50);
  u8g2.print("X:");
  printRegister(X, 70, 50);
}

// Converts the binary value stored in the register to a string and prints it at the set location
void printRegister(int registerToPrint, int xCoord, int yCoord) {
  int maxStringLength;
  String outp;
  bool registerIsNegative = false;
  
  switch (base) {
    case 10:
      maxStringLength = 6;
      outp += String(registerToPrint, DEC);
      break;
    case 16:
      maxStringLength = 6;
      outp += String(registerToPrint, HEX);
      break;
    case 8:
      maxStringLength = 6;
      outp += String(registerToPrint, OCT);
      break;
    case 2:
      maxStringLength = 16;
      outp += String(registerToPrint, BIN);
      break;
  }

  // Right aligning
  for ( int i=0; i< (outp.length() - maxStringLength); i++ ) {
    if (base == 2) {
      // Pad the binary value with 0
      outp = "0" + outp;      
    } else {
      outp = " " + outp;
    }
  }

  

  if (base == 2) {
    u8g2.setFont(u8g2_font_5x8_mn); // Use a 5x8 px font
    u8g2.setCursor(32, yCoord);

    // split into groups of 4
      outp = outp.substring(0,4) + " " +
             outp.substring(4,8) + " " +
             outp.substring(8,12) + " " +
             outp.substring(12,16);
             
    u8g2.print(outp);
  } else {
    u8g2.setFont(u8g2_font_9x15_mf);
    u8g2.setCursor(xCoord, yCoord);

    outp.toUpperCase();
    u8g2.print(outp);
  }
  
  
}




void drawStatus() {
  // Show base
  switch (base) {
    case 10:
      drawString(1, 6, "DEC", 0, false);
      break;
    case 16:
      drawString(1, 6, "HEX", 0, false);
      break;
    case 8:
      drawString(1, 6, "OCT", 0, false);
      break;
    case 2:
      drawString(1, 6, "BIN", 0, false);
      break;
  }


  

  // Status registers
  int statusX = 51;
  drawString(statusX, 6, "CRRY", 0, (lastOperationStatus & 0x01) );
  
  statusX += 20;
  drawString(statusX, 6, "ZERO", 0, (lastOperationStatus & 0x02) );

  statusX += 20;
  drawString(statusX, 6, "OVRFL", 0, (lastOperationStatus & 0x08) );

  statusX += 24;
  drawString(statusX, 6, "NEG", 0, (lastOperationStatus & 0x04) );





  // Status bar's horizontal line
  u8g2.setDrawColor(1);
  u8g2.drawHLine(0, 8, 128);
}
