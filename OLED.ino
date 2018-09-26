void printUpdateTime()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("******* ENRES *******");
  display.println();
  display.println("NB-Iot Logger v0.1");
  display.println();
  display.print("IP : ");
  display.println(ip);
  display.println();
  display.print("UL : ");
  display.print(dt.hour(), DEC);
  display.print(':');
  display.print(dt.minute(), DEC);
  display.print(':');
  display.println(dt.second(), DEC); 
  display.display();
}

void printSDError()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("******* ENRES *******");
  display.println();
  display.println("NB-Iot Logger v0.1");
  display.println();
  display.println();
  display.print("Please Check SD Card!");
  display.display();
}

void printModbusError()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("******* ENRES *******");
  display.println();
  display.println("NB-Iot Logger v0.1");
  display.println();
  display.println();
  display.print("Please Check RS-485!");
  display.display();
}
