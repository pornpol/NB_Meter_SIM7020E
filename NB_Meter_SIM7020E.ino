#include <time.h>
#include <SPIFlashMeter_MEGA.h>
#include <SIM7020E.h>

#include <RTClib.h>
#include <ModbusMeter_MEGA.h>
//#include <ModbusMaster.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define RECSIZE           64  // 40 bytes >> 64
//#define MAXMETER          3  //
#define BATCHSIZE         3

uint8_t unPostCnt = 0;
uint8_t unConnCnt = 0;

SIM7020E nb;

SPIFlashMeter flash;

String ip;
bool nbConn = false;
uint8_t unRegCnt = 0;

time_t t;

Adafruit_SSD1306 display(99);

RTC_DS3231 rtc;
DateTime dt;

// instantiate ModbusMaster object
ModbusMeter meter;
//ModbusMaster node;

#define NUM_MB_KEY  10
#define NUM_MB_DEVICE 1

#define PWR_EN_PIN  4
#define PWR_KEY_PIN 5

uint8_t lastLoop = 99;

static uint64_t lastMillis;

uint32_t wIndex = 0; 
uint32_t rIndex = 0;

//uint8_t wBuff[RECSIZE*MAXMETER];  // Max Buffer = Record Size * Max no. Meter
//uint8_t rBuff[RECSIZE*MAXMETER];

String url = "";
String path = "";
String access_token = "";
uint8_t numMeter;

uint8_t meterId;
uint32_t meterXId;
uint8_t meterType;
uint8_t meterIndex;
float meterAdjust[10];
uint16_t meterTable[11];

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);     // for Debug
  Serial2.begin(115200);  // for SIM7020E
  Serial1.begin(9600);    // for Meter

  initOLED();
  
  initMeter();

  initSD();
  readGCfg();
  for(uint8_t i=0; i<numMeter; i++)
    readMCfg(i);

  flash.begin(RECSIZE, Serial);

  // Init NB
  initNB();
  
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
  display.display();

  // Init RTC
  initRTC();
}

void loop() {
  dt = rtc.now();
  if (lastLoop != dt.minute()) {
    
///////////////////////////////////////////////////////////////////////
    Serial.println("--------------------------");
    uint32_t start = millis();
    
    if(nb.register_network()) {
      // Create HttpRequest only first
      if(nbConn == false) {
        delay(500);
        nb.destroyHttp(0);
        delay(500);
        if(nb.createHttp(url) == true) {
          Serial.println("Create HTTP Success!");
          nbConn = true;
          delay(1000);
        }
        delay(500);
      }
////////////////////////////////////
      if(nb.connectHttp(0)) {
//        delay(500);
        for(uint8_t i=0; i<numMeter;) {
          ////////////////////////////////////////////////////////////////
          uint8_t cnt = 0;
          String json;
          json += "[";
          for(uint8_t j = 0; (j < BATCHSIZE) && (i+j < numMeter); j++) {
            readMCfg(i+j);
            // Read Modbus
            readMeterData();
            cnt++;
          
            // Create Payload
            json += createPayload();
            if((j < BATCHSIZE-1) && (i+j < numMeter-1)) json += ",";
          }
          json += "]";
//          Serial.println(json);
          Serial.print("POST Data -- ");
          ////////////////////////////////////////////////////////////////
          
          if(nb.postHttp(0, path +"?access_token=" + access_token, json)) {
            Serial.println("http OK");
            i+=cnt;
            unPostCnt = 0;
          } else {
            Serial.println("http Not OK!!!");
            if(++unPostCnt == 5) {
              unPostCnt = 0;  // Reset Counter
              nbConn = false; // Reset Conn
              i = numMeter; // Exit loop
              initNB();
            }
          }
//          delay(500);
        }
        printUpdateTime();
        lastLoop = dt.minute();
      } else {
        if (++unConnCnt > 5) {
          initNB();
          unPostCnt = 0;
          unConnCnt = 0;
          nbConn = false;
        }
      }
      
      nb.disconnectHttp(0);
//////////////////////////      
    } else {
      if(++unRegCnt > 100) {
        initNB();
        unRegCnt = 0;
        unPostCnt = 0;
        unConnCnt = 0;;
      }
      nbConn = false;
      delay(2000);
    }
    Serial.print("Task : ");
    Serial.print(millis()-start);
    Serial.println(" ms");
//////////////////////////////////////////////////////////////////////////////////
    delay(500);
  }
}

float wordToFloat(uint16_t h, uint16_t l)
{
  typedef union
  {
  float number;
  uint16_t wd[2];
  } FLOATUNION_t;

  FLOATUNION_t myFloat;

  myFloat.wd[0] = l;
  myFloat.wd[1] = h;

  return myFloat.number;
}

int fp2Dec(float val)
{
  return int(round((val-int(val))*10000.0));
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/
bool readMeterData()
{
//  for(uint8_t i = 0; i<numMeter; i++)
//  {
    uint8_t result = meter.readMeterData(0, meterId, meterIndex, meterType, dt.unixtime(), meterAdjust, meterTable);
    if(!result)
    {
      Serial.print("Read Meter ID: ");
      Serial.print(meterId);
      Serial.println(" OK ^^");
    } 
    else
    {
      Serial.print("Read Meter ID: ");
      Serial.print(meterId);
      Serial.println(" ERROR !!");
      return false;
    }
    delay(50); // Must add delay when switch slave ID
//  }
  return true;
}

void initMeter()
{
  meter.begin(Serial1);
}

void initOLED()
{
  // Init OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("******* ENRES *******");
  display.println();
  display.println("NB-Iot Logger v0.1");
  display.println();
  display.println("IP : Connecting");
  display.display();  
}

void initNB()
{
  nb.init(Serial2, Serial, PWR_KEY_PIN, PWR_EN_PIN);
  nb.powerOff();
  delay(1000);
  nb.powerOn();
  nb.initModem();
  //ip = nb.check_ipaddr();
}

String createPayload()
{
  // Update to receive data from agrument
  String json;
  uint8_t i=0;

  char buffer [13];
  DateTime _dt = meter.md[i].mdt;
  sprintf (buffer, "%04u%02u%02u%02u%02u\0", _dt.year(), _dt.month(), _dt.day(), _dt.hour(), _dt.minute());
  
  json += "{\"emid\":" + String(meterXId) + ",\"mdt\":\"" + String(buffer) + "\",\"w\":" + String(meter.md[i].watt) + ",\"wh\":" + String(uint32_t(meter.md[i].wattHour)) 
                +",\"pf\":" + String(meter.md[i].pf) +",\"varh\":" + String(uint32_t(meter.md[i].varh)) 
                +",\"i0\":" + String(meter.md[i].i0) +",\"i1\":" + String(meter.md[i].i1) +",\"i2\":" + String(meter.md[i].i2) 
                +",\"v0\":" + String(meter.md[i].v0) +",\"v1\":" + String(meter.md[i].v1) +",\"v2\":" + String(meter.md[i].v2) +"}";
                
  return json;
}


