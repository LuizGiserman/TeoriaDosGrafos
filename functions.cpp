
#include "functions.h"

std::tuple <std::string, std::string> SplitString (std::string mainString , char character)
{

  std::string string1, string2;
  int position;
  unsigned int index;
  std::tuple <std::string, std::string> returnTuple

  if ((position = mainString.find(character))
  {
    for(index = mainSting.begin(); index < position; index++)
      string1.push_back(mainString[index]);
    for(index = position+1; index != mainString.end(); index++)
      string2.push_back(mainString[index]);

  }

  returnTuple = (string1, string2);
  return returnTuple;
  
}
