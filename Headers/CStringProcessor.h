#pragma once

#include "DataTypes.h"
#include "IDataProcessor.h"

template <typename PData, typename PProcessResult> class IDataStorage;
using PStringStorage = std::shared_ptr<IDataStorage<PRawData, PResult>>;

class ISubStringSearch;
using PSubStringSearch = std::shared_ptr<ISubStringSearch>;

class CStringProcessor : public IDataProcessor
{
public:
  CStringProcessor(PStringStorage, PSubStringSearch, std::string mask);
  ~CStringProcessor() override;

  void Start();
  void Join();
private:
  struct Impl;
  std::unique_ptr<Impl> m_pImpl;
};
