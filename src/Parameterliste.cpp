#include "Parameterliste.h"

const char* getParametertypName(Parametertyp parametertyp) {
  return ParametertypName[parametertyp];
}

byte getParametertypLenth(Parametertyp parametertyp){
  switch (parametertyp) {
    case Keiner:          return 0;

    case Stat01:
    case Stat0F:
    case Analog1b:        return 1;

    case Analog2b:        return 2;

    case Analog2b_Sensor: return 3;
  }
}

const Parameterelement* getParameterelement(byte parameterNr)
{
  //Performing a search over all elements
  for(byte i=0;i<parameterelementsSize;i++)
  {
    if(parameterelements[i].Nr==parameterNr)
      //returns element poointer when found
      return &parameterelements[i];
  }

  //Elsewise returns a null pointer
  return NULL;
}
