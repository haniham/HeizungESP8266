#ifndef Parameterliste_h
#define Parameterliste_h

#include <arduino.h>
#include <Stream.h>
#include <string>

//Enum for holding the Type of parameter
enum Parametertyp {Keiner=0, Stat01, Stat0F, Analog1b, Analog2b, Analog2b_Sensor,Fehlerspeicher };
// Hplding the Name for the Enums
extern const char* ParametertypName[];
//Returning the Name of the Enum
const char* getParametertypName(Parametertyp parametertyp);

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
  {2,"Sollwert_Brauchwasser",Analog2b},
  {4,"Sollwert_Speicher",Analog2b},
  {5,"Flammsignal",Stat0F},
  {6,"Fehlerspeicher",Fehlerspeicher},
  {8,"SommerWinter",Stat0F},
  {14,"Stat_Raumthermostat",Stat01},
  {22,"Brauchwassersensor",Analog2b_Sensor},
  {23,"Speichertemperatur",Analog2b_Sensor},
  {24,"Vorlauftemperatur",Analog2b_Sensor},
  {31,"Anzahl_GFA_Stoerungen",Analog1b},
  {32,"Anzahl_STB_Abschaltungen",Analog1b},
  {36,"Drezahlsollwert",Analog2b},
  {37,"Sollwert_Vorlauf_Ext_Regler",Analog2b},
  {38,"Fehlerspeicher",Fehlerspeicher},
  {56,"Verbleibende_Brennersperrzeit",Analog1b},
  {57,"Sollwert_Vorlauf",Analog2b},
  {63,"Externe_Heizungspumpe",Stat01},
  {66,"Stellung_VUV",Analog1b},
  {68,"Interne_Heizungspumpe",Stat01},
  {88,"Anforderung_Warmwasser_C1C2",Stat01},
  {99,"Wasserpumpennachlaufzeit_nach_Speicherladung",Analog1b},
  {100,"Wasserpumpennachlaufzeit_nach_Heizbetrieb",Analog1b},
  {101,"Korrekturwert_Aussentemperatur",Analog1b},
  {102,"Speicherladezeit",Analog1b},
  {106,"Aussentemperatur",Analog2b_Sensor},
  {107,"Ext_VorRuecklauftemp",Analog2b_Sensor},
  {108,"Heizungszeillast",Analog1b},
  {111,"Zaehler_3_Zuendversuche",Analog1b},
  {113,"Speicherfreigab_Schaltuhr",Stat01},
  {131,"Drezahlistwert",Analog2b}

};
const byte parameterelementsSize = sizeof (parameterelements) / sizeof *(parameterelements);

/*
/ Searches the Parameterelements for the corresponding element
/ Returns NULL if not found
*/
const Parameterelement* getParameterelement(byte parameterNr);
#endif
