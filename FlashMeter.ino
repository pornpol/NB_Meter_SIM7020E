//bool meterToFlash()
//{
//  uint16_t index = 0;
//  // mdt, w, wh, pf, varh, i0, i1, 12, v0, v1, v2
//  for(int i=0; i<sd.cfgG.numMeter; i++)
//  {
//    *(uint32_t*)&wBuff[index] = sd.cfgM[i].xid;   //[0, 4,294,967,295]
//    index += 4;
//    *(uint32_t*)&wBuff[index] = meter.md[i].mdt;  //[0, 4,294,967,295]
//    index += 4;
// 
//    *(float*)&wBuff[index] = meter.md[i].watt;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].wattHour;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].pf;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].varh;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].i0;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].i1;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].i2;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].v0;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].v1;
//    index += 4;
//    *(float*)&wBuff[index] = meter.md[i].v2;
//    index += 4;
//    
//    for(int i = index; (i%RECSIZE)>0; i++) //Zero padding
//    {  
//        wBuff[index] = 0;
//        index++;
//    }
//  }
//  
//  Serial.printf("Write Index : %d\r\n", wIndex);
//
//  if(flash.writeMeterData(wIndex, wBuff, RECSIZE*sd.cfgG.numMeter))
//  {
//    fErrorCnt = 0;
//    Serial.println("Flash Write OK");
//    wIndex += sd.cfgG.numMeter;
//    // Save to RTC
//    setWIndex();
//    Serial.printf("Next Write Index : %d\r\n", wIndex);
//    UNDERLINE;
//    errlog.e.spiflash = 0;
//    fErrorCnt = 0;
//  }
//  else 
//  {
//    Serial.println("Flash Write Not OK!!!!");
//    UNDERLINE;
//    errlog.e.spiflash = 1;
//
//    fErrorCnt++;
//    if((fErrorCnt >= FLASH_ERROR_MAX) && (wIndex == rIndex))
//    {
//      Serial.println("Move Flash Index to next sector!!!!");
//      fErrorCnt = 0;
//      wIndex = ((wIndex/64)+1)*64;
//      rIndex = wIndex;
//      setRIndex();
//      setWIndex();
//    }
//  }
//  
//  return true;
//}
