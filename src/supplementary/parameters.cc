#include "../../include/supplementary/parameters.h"
/*
*class for storage parameters
*type could be: double, int, bool, string
*/

void parametersClass::setDouble(std::string param, double value){
  pDouble[param.c_str()]=value;
  printf("%s:%12.6e\n", param.c_str(), value);  
}
void parametersClass::setInt(std::string param, int value){
  pInt[param.c_str()]=value;
  printf("%s:%u\n", param.c_str(), value);
}
void parametersClass::setBool(std::string param, bool value){
  pBool[param.c_str()]=value;
  printf("%s:%u\n", param.c_str(), value);
}
void parametersClass::setString(std::string param, std::string value){
  pString[param.c_str()]=value;
  printf("%s:%s\n", param.c_str(), value.c_str());
}
double parametersClass::getDouble(std::string param){
  if (pDouble.count(param.c_str())==0){printf("unknown parameter '%s' requested\n", param.c_str()); exit(-1);}
  return pDouble[param.c_str()];
}
int parametersClass::getInt(std::string param){
  if (pInt.count(param.c_str())==0){printf("unknown parameter '%s' requested\n", param.c_str()); exit(-1);}
  return pInt[param.c_str()];
}
bool parametersClass::getBool(std::string param){
  if (pBool.count(param.c_str())==0){printf("unknown parameter '%s' requested\n", param.c_str()); exit(-1);}
  return pBool[param.c_str()];
}
std::string parametersClass::getString(std::string param){
  if (pString.count(param.c_str())==0){printf("unknown parameter '%s' requested\n", param.c_str()); exit(-1);}
  return pString[param.c_str()];
}
/*
void parametersClass::readInParameters(std::string fileName){
  //read parameters from file
  printf("\nreading parameter file: %s\n", fileName.c_str());
  std::ifstream inputFile(fileName.c_str());
  std::string line;
  while (std::getline(inputFile, line)){
    std::istringstream ss(line);
    std::string key; ss >> key;
    if (pInt.find(key)!=pInt.end()) {
      int temp;
      ss >> temp; setInt(key, temp);
    }
    else if (pDouble.find(key)!=pDouble.end()) {
      double temp;
      ss >> temp; setDouble(key, temp);
    }
    else if (pBool.find(key)!=pBool.end()) {
      bool temp;
      ss >> temp; setBool(key, temp);
    }
    else if (pString.find(key)!=pString.end()) {
      std::string temp;
      ss >> temp; setString(key, temp);
    }
    else {
      std::cout << "unknown parameter '" << key << "' in parameter file\n"; exit(-1);
    }
  }
  printf("\n");
}
*/


