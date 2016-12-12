/*
* Provide parameters storage
*/

#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <iostream>
#include <sstream>
#include <string.h>
#include <map>
class parametersClass{
 public:
  void setDouble(std::string param, double value);
  void setInt(std::string param, int value);
  void setBool(std::string param, bool value);
  void setString(std::string param, std::string value);
  double getDouble(std::string param);
  int getInt(std::string param);
  bool getBool(std::string param);
  std::string getString(std::string param);
  //void readInParameters(std::string fileName);
 private:
  std::map<std::string, int> pInt;
  std::map<std::string, double> pDouble;
  std::map<std::string, bool> pBool;
  std::map<std::string, std::string> pString;
};

#endif