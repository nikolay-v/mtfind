#pragma once

#include "DataTypes.h"

class ISubStringSearch
{
public:
  virtual ~ISubStringSearch(){}

  virtual PResult Search(const std::string& pData, const std::string& pMask) = 0;
};
