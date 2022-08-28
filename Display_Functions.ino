
/**
 * Draws the softkeys along the bottom edge of the screen with a label. 
 * @param {int} keyIndex The key number 0 thru 4.
 * @param {String} keyLabel The key label, only the first 5 characters are printed.
 * @param {bool} hasSubKeys Changes style to show there are nested sub-keys.
 */
void drawSoftKey(int keyIndex, String keyLabel, bool hasSubKeys) {
  int keyWidth = 24;
  int keyHeight = 8;
  int screenBottom = u8g2.getDisplayHeight() - 1;
  
  int x = 26 * keyIndex; // X offset of the key calculated by it's index
   
  u8g2.setDrawColor(1);

  /*    ___________
   *   /           \
   *   |            |
   */

  u8g2.drawVLine(x,            screenBottom-keyHeight+3, keyHeight-2); // left line
  u8g2.drawVLine(x+keyWidth-1, screenBottom-keyHeight+3, keyHeight-2); // right line

  u8g2.drawHLine(x+2, screenBottom-keyHeight+1, keyWidth-4); // top line

  u8g2.drawPixel(x+1, screenBottom-keyHeight+2); // top left corner
  u8g2.drawPixel(x+keyWidth-2, screenBottom-keyHeight+2); // top right corner

  // Fill in the lower right corner to show more
  if (hasSubKeys) {
    u8g2.drawPixel(x+keyWidth-2, screenBottom-1);
    u8g2.drawPixel(x+keyWidth-2, screenBottom);
    u8g2.drawPixel(x+keyWidth-3, screenBottom);
  }

  drawString(x+2, screenBottom, keyLabel, 0, false);
}


/**
 * Draw the string at the given coordinates and size, optionally choose to draw an inverted box over the text
 * @param {int} x The string X coordinate.
 * @param {int} y The string Y coordinate of the text baseline.
 * @param {String} stringToDraw A string containing the text to draw.
 * @param {int} fontSize Se the font size from 0-3 for the 4 available system font sizes small to large.
 */
void drawString(int x, int y, String stringToDraw, int fontSize, bool drawInverted) {
  switch (fontSize) {
    case 2:
      u8g2.setFont(u8g2_font_crox3h_tf); // Large 12px (was u8g2_font_freedoomr10_tu)
      break;
    case 1:
      u8g2.setFont(u8g2_font_t0_11_tf); // Small 8px 
      break;
    default:
      u8g2.setFont(u8g2_font_micro_tr); // Extra small 5px - keep
  }

  u8g2.setDrawColor(1);
  u8g2.setCursor(x, y);
  u8g2.print(stringToDraw);

  if (drawInverted) {
    u8g2.setDrawColor(2);
    //int stringWidth = u8g2.getStrWidth(stringToDraw.c_str());
    u8g2.drawBox(x-1, y-u8g2.getMaxCharHeight()-1, u8g2.getStrWidth(stringToDraw.c_str())+2, u8g2.getMaxCharHeight()+2);
  }
}




int asciiToInt(char asciiIn) {
  int returnVal = 0;
  
  // Convert the ASCII codes into numbers 0 to F
  if ((asciiIn >= 48) && (asciiIn <= 57)) { // numbers 0 to 9 in ASCII
    returnVal = asciiIn - 48;
  } else if ((asciiIn >= 97) && (asciiIn <= 102)) { // characters a to f
    returnVal = asciiIn - 87;
  }
  return returnVal;
}
