#include "vaillant.h"

// checked
byte calculateCRC(byte* stringptr, byte len) {
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

    checksum = checksum ^ stringptr[i];
  }
  return checksum;
}

char getNumChar(byte num) {
  if(num<=9)
    return num + '0';
  if(num<=15)
    return num-10 + 'A';
  else
    return '0';
}

boolean readParam(byte paramNr,char* stringptr, byte stringlen) {
  SerialDebug.print("Reading Parameter");
  SerialDebug.println(paramNr);

  //Make the Request
  byte request[7] = {7,0,  0,0,paramNr,5, '\0'};
  request[6] = calculateCRC(request,6);

  //Sending Request
  int i =0;
  for(;i<7;i++)
    SerialVaillant.write(request[i]);

  Stream &streamVaillant = SerialVaillant;
  streamVaillant.setTimeout(1000);
  byte receiveBuffer[50];

  //read first Byte
  if(streamVaillant.readBytes(receiveBuffer,1)!=1) {
    SerialDebug.println("nothing receieved");
    return false;
  }

  byte transmissionLenth = receiveBuffer[0];

  if(transmissionLenth>50) {
    SerialDebug.println("bigger 50");
    return false;
  }

  byte awaitedBytes = transmissionLenth-1;

  SerialDebug.print("Awaiting ");
  SerialDebug.print(awaitedBytes);
  SerialDebug.println("bytes");

  if(streamVaillant.readBytes(&(receiveBuffer[1]),awaitedBytes)!=awaitedBytes) {
    SerialDebug.print("nothing receieved2");
    return false;
  }

  int j =0;
  for(;j<transmissionLenth;j++){
    int j3= 3*j;
    stringptr[j3]=getNumChar(receiveBuffer[j]>>4);
    stringptr[j3+1]=getNumChar(receiveBuffer[j]&0xF);
    stringptr[j3+2]=' ';

    if((j3+5)>(stringlen-1))
      break;
  }
  stringptr[j*3]='\0';
  stringptr[stringlen-1] = '\0';
  SerialDebug.println(stringptr);

  return true;


}
