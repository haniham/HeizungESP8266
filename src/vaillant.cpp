#include "vaillant.h"

// checked
byte calculateCRC(byte* data, byte len) {
  byte checksum = 0;
  byte i = 0;
  for(i=0;i<len;i++)
  {
    if(checksum&0x80)
    {
      checksum= (checksum<<1 )|1;
      checksum = checksum ^ 0x18;
    }
    else
    {
      checksum = checksum<<1;
    }

    checksum = checksum ^ data[i];
  }
  return checksum;
}

boolean checkCRC(byte* data, byte len) {
  return calculateCRC(data, len-1) == data[len-1];
}

char getNumChar(byte num) {
  if(num<=9)
    return num + '0';
  if(num<=15)
    return num-10 + 'A';
  else
    return '0';
}

void writeDebugBytes(byte* data, byte dataLenth) {
  for(byte i = 0; i<dataLenth;i++) {
    SerialDebug.print(data[i],HEX);
    SerialDebug.print(' ');
  }
}

byte readParam(byte paramNr,byte* resultBytePtr, byte resultByteLength) {
  // For a Parameter Request the debug Byte is 0
  return readRequest(0,paramNr, resultBytePtr, resultByteLength);
}

byte readRequest(byte debugByte, byte paramNr,byte* resultBytePtr, byte resultByteLength) {
  if(debugByte==0)
    SerialDebug.print("Reading Parameter");
  else
    SerialDebug.print("General Read Request");

  SerialDebug.println(paramNr);

  //Make the Request
  byte request[readParamRequestLenth] = {0x07,0x00,debugByte,0x00,paramNr,0x05, 0x00};
  request[readParamRequestLenth-1] = calculateCRC(request,readParamRequestLenth-1);
  SerialDebug.print("Request: ");
  writeDebugBytes(request, readParamRequestLenth);
  SerialDebug.println();

  //Sending Request
  for(byte i = 0; i<7; i++)
    SerialVaillant.write(request[i]);

  SerialVaillant.setTimeout(1000);

  //Read first Byte
  if(SerialVaillant.readBytes(resultBytePtr,1)!=1) {
    SerialDebug.println("nothing receieved");
    return 0;
  }

  //Receive length calculation
  byte receiveLength = resultBytePtr[0];
  if(receiveLength>resultByteLength) {
    SerialDebug.print("Vaillant wants to send ");
    SerialDebug.print(receiveLength);
    SerialDebug.print(" bytes, but only ");
    SerialDebug.print(resultByteLength);
    SerialDebug.println(" bytes as buffer");

    receiveLength = resultByteLength;
  }

  byte awaitedBytes = receiveLength-1;

  SerialDebug.print("Awaiting ");
  SerialDebug.print(awaitedBytes);
  SerialDebug.println(" additional bytes");

  //Check Length
  byte receivedBytes2 = SerialVaillant.readBytes(&(resultBytePtr[1]),awaitedBytes);
  if(receivedBytes2!=awaitedBytes) {
    SerialDebug.print("Awaited ");
    SerialDebug.print(awaitedBytes);
    SerialDebug.print(" bytes, but received ");
    SerialDebug.print(receivedBytes2);
    SerialDebug.println(" before Timeout");
    return (receivedBytes2+1)*-1;
  }

  //Final CRC Check
  if(checkCRC(resultBytePtr, receivedBytes2+1)) {
    return receivedBytes2+1;
  } else {
    SerialDebug.print("CRC Fail");
    return (receivedBytes2+1)*-1;
  }
}

String parse1ByteAnalog(byte data){
  return String(data);
}

const String parse1ByteSensorstatus(byte data) {
  switch (data) {
    case 0x00: return "kein Fehler";
    case 0x55: return "Kurzschluss";
    case 0xAA: return "Unterbrechung";
  }
  return "NaN";
}

const String parse1ByteSchaltzustand(byte data) {
  switch (data) {
    case 0xF0:
    case 0x00: return "Inaktiv";
    case 0x0F:
    case 0x01: return "Aktiv";
  }
  return "NaN";
}

const String parse2Bytes(byte* data) {
  int16_t number = (data[0]<<8)|data[1];
  return String(number/(16.0f));
}

const ParseResult parse3Bytes(byte* data) {
  return ParseResult{parse2Bytes(data),parse1ByteSensorstatus(data[2]),""};
}

//Evaluates the position of the set MSB
uint8_t getSetMSBPosition(byte* data) {
  for(uint8_t i = 0;i<=9;i++) {
    if(data[i] & 0x80)
      return i+1;
  }
  return 0;
}

const ParseResult parseFehlerspeicher(byte* data) {
  uint8_t setMSBPosition = getSetMSBPosition(data);

  char fehlerspeicher[]="00 11 22 33 44 55 66 77 88 99";
  for(int8_t i=0, pos=setMSBPosition-1;i<=9;i++,pos--) {
    if(pos<0)
      pos+=10;

    byte fehlerspeicherelement =  data[pos];

    const char* hexchars= "0123456789ABCDEF";
    fehlerspeicher[i*3]= hexchars[fehlerspeicherelement>>4];
    fehlerspeicher[i*3+1]= hexchars[fehlerspeicherelement&0xF];
  }

  return ParseResult{fehlerspeicher,String(setMSBPosition)};
}

ParseResult parseTelegram(byte ParameterNr, Parametertyp parametertyp, byte* telegramData, byte telegramLength){
  byte dataLength = telegramLength - 3;

  //nothing todo when keiner
  if(parametertyp == Keiner){
    char ErrorKeiner[] = "Parametertyp Keiner - Nothing TODO";
    SerialDebug.println(ErrorKeiner);
    return ParseResult{"","",ErrorKeiner};
  }

  //Check length of received Data
  if(dataLength!=getParametertypLenth(parametertyp)) {
    String errorParameter = "Parametertyp Lenth mismatch\n\r";
    errorParameter += getParametertypName(parametertyp);
    errorParameter += " requires ";
    errorParameter += getParametertypLenth(parametertyp);
    errorParameter += " bytes, but telegram only contains ";
    errorParameter += dataLength;
    errorParameter += " bytes";
    SerialDebug.println(errorParameter);
    return ParseResult{"","",errorParameter};
  }

  switch(parametertyp) {
      case Stat01:
      case Stat0F: return ParseResult{"",parse1ByteSchaltzustand(telegramData[2]),""};

      case Analog1b:  return ParseResult{parse1ByteAnalog(telegramData[2]),"",""};
      case Analog2b:  return ParseResult{parse2Bytes(&telegramData[2]),"",""};
      case Analog2b_Sensor: return parse3Bytes(&telegramData[2]);

      case Fehlerspeicher: return parseFehlerspeicher(telegramData);
  }
}
