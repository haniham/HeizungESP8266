#ifndef vaillant_h
#define vaillant_h

#include <arduino.h>
#include <Stream.h>
#include "Parameterliste.h"

#define SerialVaillant Serial
#define SerialDebug Serial1
#define readParamRequestLenth 7

struct ParseResult{ String value; String state; String error;};

byte calculateCRC(byte* data, byte len);
boolean checkCRC(byte* data, byte len);

/*
* Starts a request to the Vaillant Heater
*/
byte readRequest(byte debugByte, byte paramNr,byte* resultBytePtr, byte resultByteLength);
/*
* Starts a Parameter Request to the Vaillant Heater
*/
byte readParam(byte paramNr,byte* resultBytePtr, byte resultByteLength);

ParseResult parseParam(byte paramNr, byte* data, byte dataLength);

/*
/ Parses the telegram and returns the String
/ Returns an empty string if unsuccessful
*/
ParseResult parseTelegram(byte ParameterNr, Parametertyp parametertyp, byte* telegramData, byte telegramLength);

#endif
