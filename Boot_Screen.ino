unsigned long previousMillis = 0;
const long interval = 2000; 

void drawBootScreen() {
  unsigned long currentMillis = millis();
  u8g2.drawXBM(47, 19, sb_solid_width, sb_solid_height, sb_solid_bits);
  
  if (currentMillis - previousMillis >= interval) {
    currentMode = REGISTER_DISPLAY;
  }
}
