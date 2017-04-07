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

  SerialDebug.print("Reading Parameter");
  SerialDebug.println(paramNr);

  //Make the Request
  byte request[readParamRequestLenth] = {0x07,0x00,0x00,0x00,paramNr,0x05, 0x00};
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
//TODO
//String parse(byte[])

String parse1ByteSensorstatus(byte data) {
  switch (data) {
    case 0x00: return "kein Fehler";
    case 0x55: return "Kurzschluss";
    case 0xAA: return "Unterbrechung";
  }
  return "NaN";
}

String parse1ByteSchaltzustand(byte data) {
  switch (data) {
    case 0xF0:
    case 0x00: return "Inaktiv";
    case 0x0F:
    case 0x01: return "Aktiv";
  }
  return "NaN";
}

String parse2Bytes(byte* data) {
  int16_t number = (data[0]<<8)|data[1];
  return String(number/(16.0f));
}

String parse3Bytes(byte* data) {
  return parse2Bytes(data) + parse1ByteSensorstatus(data[2]);
}

String parseParam(byte paramNr, byte* data, byte dataLength){
  byte offsetFreeDataLength = dataLength - 3;
  byte* offsetFreeData = &(data[2]);

  String schaltzustand;
  switch (offsetFreeDataLength) {
    case 1:
      schaltzustand = parse1ByteSchaltzustand(offsetFreeData[0]);
      if(schaltzustand.equals("NaN")){
        return parse1ByteAnalog(offsetFreeData[0]);
      } else {
        return schaltzustand;
      }
    case 2:
      return parse2Bytes(offsetFreeData);
    case 3:
      return parse3Bytes(offsetFreeData);
    default:
    return "NaN";
  }

}
