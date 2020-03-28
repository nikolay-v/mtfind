#pragma once

#include "ISubStringSearch.h"

class CSubStringSearch : public ISubStringSearch
{
public:

  PResult Search(const std::string& pData, const std::string& pMask) override;
};
