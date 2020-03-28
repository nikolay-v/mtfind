#pragma once

#include "DataTypes.h"
#include "IDataStorage.h"
#include "IRawDataSupplier.h"

class CStringStorage : public IDataStorage<PRawData, PResult>
{
public:
  CStringStorage(PRawDataSupplier<PRawData>);
  ~CStringStorage() override;

  std::pair<PRawData, PDataInfo> GetData() override;
  void ReturnResult(PDataInfo, PResult) override;

  void Start() override;
  void Join() override;

  TResultList& GetResults() override;

private:
  struct Impl;
  std::unique_ptr<Impl> m_pImpl;
};
