#ifndef Parameterliste_h
#define Parameterliste_h

#include <arduino.h>
#include <Stream.h>
#include <string>

//Enum for holding the Type of parameter
enum Parametertyp {Keiner, Stat01, Stat0F, Analog1b, Analog2b, Analog2b_Sensor };

/*
/ returns length of the Parametertyp
/ 0 for undefined length
*/
byte getParametertypLenth(Parametertyp parametertyp);

//Structure holding the Parameter Number, its Name and the PArameter Type
struct Parameterelement{
  byte Nr;
  String Name;
  Parametertyp parametertyp;
};

//Array holding all parameteremlements
const Parameterelement parameterelements[] = {
  {4,"Sollwert_Speicher",Analog2b},
  {5,"Flammsignal",Stat0F},
  {23,"Speichertemperatur",Analog2b_Sensor},
  {24,"Vorlauftemperatur",Analog2b_Sensor},
  {57,"Sollwert_Vorlauf",Analog2b},
  {68,"Interne_Heizungspumpe",Stat01}
};
const byte parameterelementsSize = sizeof (parameterelements) / sizeof *(parameterelements);

/*
/ Searches the Parameterelements for the corresponding element
/ Returns NULL if not found
*/
const Parameterelement* getParameterelement(byte parameterNr);
#endif
