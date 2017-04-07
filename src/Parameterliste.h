#ifndef Parameterliste_h
#define Parameterliste_h

#include <arduino.h>
#include <Stream.h>
#include <string>

enum Parametertyp {Keiner, Stat01, Stat0F, Analog1b, Analog2b, Analog2b_Sensor };

class ParameterBeschreibung {
    Parametertyp parametertyp;
    std::string beschreibung;
};

Parametertyp getParametertyp(byte ParameterNr);
String getParameterBeschreibung(byte ParameterNr);

#endif
