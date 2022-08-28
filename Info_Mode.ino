void drawInfoScreen() {
  u8g2.drawXBM(2, 2, sb_solid_width, sb_solid_height, sb_solid_bits);
  drawString(45, 15, "SB116", 2, false);
  drawString(45, 26, "PROGRAMMER", 1, false);


  drawString(2, 52, "FIRMWARE VERSION 1.0.0", 0, false);
  drawString(2, 60, "(C) 2022 SB ELECTRONICS LTD.", 0, false);
}
