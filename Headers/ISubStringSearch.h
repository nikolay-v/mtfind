#pragma once

#include "DataTypes.h"

class ISubStringSearch
{
public:
  virtual ~ISubStringSearch(){}

  virtual PResult Search(std::string& pData, std::string& pMask) = 0;
};
