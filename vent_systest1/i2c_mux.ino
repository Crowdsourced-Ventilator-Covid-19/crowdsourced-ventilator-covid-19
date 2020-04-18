
 
void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
 
 
// standard Arduino setup()
void tcascan()
{
  tft.println("Running TCAscanner...");
  for (uint8_t t=0; t<8; t++) {
    tcaselect(t);
    tft.print("TCA Port #"); tft.println(t);

    for (uint8_t addr = 0; addr<=127; addr++) {
      if (addr == TCAADDR) continue;
    
      uint8_t data;
      if (! twi_writeTo(addr, &data, 0, 1, 1)) {
         tft.print("Found I2C 0x");  tft.println(addr,HEX);
      }
    }
  }
  tft.println("done");
  tft.println("");
}
