#include <SPI.h>
#include <SD.h>
#include "ArduinoJson.h"

const int chipSelect = 48;

File myFile;

void initSD()
{
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    printSDError();
    for(;;);
    //return;
  }
  Serial.println("card initialized.");
}

String readConfig() {
  myFile = SD.open("cfg");
  if (myFile) {
    String cfg;
  
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
//      Serial.write(myFile.read());
      cfg += char(myFile.read());
    }
    // close the file:
    myFile.close();
    return cfg;
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening cfg.json");
    return "";
  }
}

//bool phaseCfg(String cfg) {
//  DynamicJsonBuffer jsonBuffer;
//  JsonObject& root = jsonBuffer.parseObject(cfg);
//
////  Serial.print(cfg);
//  
//  url = root["SERVER_API_ENDPOINT"]["url"].as<String>();
//  path = root["SERVER_API_ENDPOINT"]["path"].as<String>();
//  access_token = root["SERVER_API_ENDPOINT"]["access_token"].as<String>();
//  numMeter = root["SERVER_API_ENDPOINT"]["meters"];
//
//  for(uint8_t i=0; i<numMeter; i++)
//  {
//    meterId[i]    = root["METER_CONFIG"]["config"][i]["id"];
//    meterXId[i]   = root["METER_CONFIG"]["config"][i]["xid"];
//    meterType[i]  = root["METER_CONFIG"]["config"][i]["type"];
//    meterIndex[i]  = root["METER_CONFIG"]["config"][i]["index"];
//    for(uint8_t j=0; j<10; j++)
//    {
//      meterAdjust[i][j] = root["METER_CONFIG"]["config"][i]["adjust"][j];
//      meterTable[i][j] = root["METER_CONFIG"]["config"][i]["table"][j];
//    }
//    meterTable[i][10] = root["METER_CONFIG"]["config"][i]["table"][10]; // Modbus Function
//  }
//  return true;
//}

void readGCfg()
{
  myFile = SD.open("cfg");
  if (myFile) {
    String cfg;
  
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      cfg += char(myFile.read());
    }
    myFile.close();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(cfg);
    
    url = root["SERVER_API_ENDPOINT"]["url"].as<String>();
    path = root["SERVER_API_ENDPOINT"]["path"].as<String>();
    access_token = root["SERVER_API_ENDPOINT"]["access_token"].as<String>();
    numMeter = root["SERVER_API_ENDPOINT"]["meters"];
  
    return true;
  } else {
    
    // if the file didn't open, print an error:
    Serial.println("error opening cfg.json");
    return false;
  }  
}

void readMCfg(uint8_t i)
{
  myFile = SD.open(String(i));
  if (myFile) {
    String cfg;
  
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      cfg += char(myFile.read());
    }
    myFile.close();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(cfg);
    
    meterId    = root["METER_CONFIG"]["id"];
    meterXId   = root["METER_CONFIG"]["xid"];
    meterType  = root["METER_CONFIG"]["type"];
    meterIndex  = root["METER_CONFIG"]["index"];
    for(uint8_t j=0; j<10; j++)
    {
      meterAdjust[j] = root["METER_CONFIG"]["adjust"][j];
      meterTable[j] = root["METER_CONFIG"]["table"][j];
    }
    meterTable[10] = root["METER_CONFIG"]["table"][10]; // Modbus Function

    return true;
  } else {
    
    // if the file didn't open, print an error:
    Serial.println("error opening cfg.json");
    return false;
  }    
}

