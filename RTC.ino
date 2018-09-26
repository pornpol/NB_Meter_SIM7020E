#define SRAM_ADDRESS      DS3231_NVRAM // Change to DS3232

void getWIndex()
{
  rtc.readnvram((uint8_t*)&wIndex, 4, SRAM_ADDRESS+1);
}

void setWIndex()
{
  rtc.writenvram(SRAM_ADDRESS+1, (uint8_t*)&wIndex, 4);
}

void getRIndex()
{
  rtc.readnvram((uint8_t*)&rIndex, 4, SRAM_ADDRESS+5);
}

void setRIndex()
{
  rtc.writenvram(SRAM_ADDRESS+5, (uint8_t*)&rIndex, 4);
}

void initRTC()
{
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // Test Backup Index
  if(rtc.readnvram(SRAM_ADDRESS) != 0x55)
  {
    Serial.println(rtc.readnvram(SRAM_ADDRESS));
    Serial.println("Init NVRAM Backup");
    rtc.writenvram(SRAM_ADDRESS, 0x55);

    setWIndex();
    setRIndex();
  }
  else
  {
    getWIndex();
    getRIndex();
  }
}

