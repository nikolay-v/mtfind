#include <thread>

#include "IDataStorage.h"
#include "ISubStringSearch.h"

#include "CStringProcessor.h"

struct CStringProcessor::Impl
{
  Impl(PStringStorage pStorage, PSubStringSearch pSeacher, std::string mask)
    : m_pStorage(pStorage)
    , m_pSeacher(pSeacher)
    , m_mask(mask)
  {}
  void Start();
  void Join();

private:
  PStringStorage m_pStorage;
  PSubStringSearch m_pSeacher;
  std::string m_mask;
  std::unique_ptr<std::thread> m_pThread;
};

CStringProcessor::CStringProcessor(PStringStorage pStorage, PSubStringSearch pSeacher, std::string mask)
  : m_pImpl(std::make_unique<CStringProcessor::Impl>(pStorage, pSeacher, mask)) {}

CStringProcessor::~CStringProcessor() {}

void CStringProcessor::Start()
{
  m_pImpl->Start();
}

void CStringProcessor::Join()
{
  m_pImpl->Join();
}

void CStringProcessor::Impl::Start()
{
  m_pThread = std::make_unique<std::thread>([&]()
    {
      for (;;)
      {
        auto res = m_pStorage->GetData();
        if (res.second->m_result != m_pStorage->E_OK)
          return;

        auto searchRes = m_pSeacher->Search(res.first->m_data, m_mask);

        searchRes->m_line = res.first->m_uid.m_lineIndex;
        auto blockPos = res.first->m_uid.m_pos;
        if (blockPos != 0)
          for (auto& unit : searchRes->m_results)
            unit.m_pos += blockPos;

        m_pStorage->ReturnResult(res.second, searchRes);
      }
    });
}

void CStringProcessor::Impl::Join()
{
  m_pThread->join();
}
