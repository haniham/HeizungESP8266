#include "Parameterliste.h"

Parameterelement* getParameterelement(byte parameterNr)
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
