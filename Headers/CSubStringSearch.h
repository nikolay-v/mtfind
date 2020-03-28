#pragma once

#include "ISubStringSearch.h"

class CSubStringSearch : public ISubStringSearch
{
public:

  PResult Search(std::string& pData, std::string& pMask) override;
};
