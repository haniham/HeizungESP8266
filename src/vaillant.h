#ifndef vaillant_h
#define vaillant_h

#include <arduino.h>
#include <Stream.h>

#define SerialVaillant Serial
#define SerialDebug Serial1

byte calculateCRC(byte* stringptr, byte len);
boolean readParam(byte paramNr,char* stringptr,byte stringlen);

#endif
