#include "Parameterliste.h"

Parametertyp getParametertyp(byte ParameterNr){
  switch (ParameterNr) {
    case 4: return Analog2b;
    case 5: return Stat0F;
    case 23: return Analog2b_Sensor;
    case 24: return Analog2b_Sensor;
    case 57: return Analog2b;
    case 68: return Stat01;
  }
}

String getParameterBeschreibung(byte ParameterNr)
{
  switch (ParameterNr){
    case 4: return "Sollwert_Speicher";
    case 5: return "Flammsignal";
    case 23: return "Speichertemperatur";
    case 24: return "Vorlauftemperatur";
    case 57: return "Sollwert_Vorlauf";
    case 68: return "Interne_Heizungspumpe";
  }
  return "";
}
