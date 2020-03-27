#include <assert.h>
#include <condition_variable>
#include <thread>
#include <mutex>

#include "CStringStorage.h"

struct CStringStorage::Impl
{
  Impl(PRawDataSupplier<PRawData> pSupplier) : m_pSupplier(pSupplier) {}

  std::pair<PRawData, PDataInfo> GetData();
  void ReturnResult(PDataInfo, PResult);

  void Start();
  void Join();

  TResultList& GetResults();

private:
  PRawDataSupplier<PRawData> m_pSupplier;

  std::unique_ptr<std::thread> m_pThread;

  static const unsigned MAX_RAW_LOADED = 10;
  unsigned m_rawLoaded = 0;
  std::mutex m_RawDataMutex;
  std::condition_variable m_FreeSpace;
  std::condition_variable m_Filled;
  std::list<std::pair<PRawData, TDataId>> m_rawDataList;
  bool PutRawData(PRawData, TDataId);
  bool GetRawData(PRawData&, TDataId&);
  bool m_eof = false;

  TDataId m_lineCounter = 0;
  std::mutex m_ResultsMutex;
  using TResultList = std::list<std::pair<TDataId, PResult>>;
  TResultList m_results;
  TResultList::iterator m_uncompleted = m_results.begin();
  TDataId CreateResultRecord();
  void PutResult(TDataId, PResult);
};

CStringStorage::CStringStorage(PRawDataSupplier<PRawData> pSupplier)
  : m_pImpl(std::make_unique<Impl>(pSupplier))
{
}

CStringStorage::~CStringStorage()
{
}

std::pair<PRawData, CStringStorage::PDataInfo> CStringStorage::GetData()
{
  return m_pImpl->GetData();
}

void CStringStorage::ReturnResult(PDataInfo dataInfo, PResult result)
{
  m_pImpl->ReturnResult(dataInfo, result);
}

void CStringStorage::Start()
{
  m_pImpl->Start();
}

void CStringStorage::Join()
{
  m_pImpl->Join();
}

CStringStorage::TResultList& CStringStorage::GetResults()
{
  return m_pImpl->GetResults();
}

std::pair<PRawData, CStringStorage::PDataInfo> CStringStorage::Impl::GetData()
{
  PRawData pData;
  TDataId id;
  std::unique_lock<std::mutex> lock(m_RawDataMutex);
  for (;;)
  {
    if (GetRawData(pData, id))
      break;

    if (m_eof)
      return std::make_pair(pData, std::make_shared<TDataInfo>(E_END_OF_DATA, 0));

    m_Filled.wait(lock);
  }

  m_FreeSpace.notify_one();

  return std::make_pair(pData, std::make_shared<TDataInfo>(E_OK, id));
}

void CStringStorage::Impl::ReturnResult(PDataInfo dataInfo, PResult result)
{
  std::unique_lock<std::mutex> lock(m_ResultsMutex);
  PutResult(dataInfo->m_id, result);
}

void CStringStorage::Impl::Start()
{
  m_pThread = std::make_unique<std::thread>([&]()
    {
      for (;;)
      {
        auto retVal = m_pSupplier->GetData();
        if (retVal.second != m_pSupplier->E_OK)
        {
          m_eof = true;
          break;
        }

        TDataId uid;
        {
          std::unique_lock<std::mutex> lock(m_ResultsMutex);
          uid = CreateResultRecord();
        }

        for (;;)
        {
          std::unique_lock<std::mutex> lock(m_RawDataMutex);
          if (PutRawData(retVal.first, uid))
            break;

          m_FreeSpace.wait(lock);
        }

        m_Filled.notify_one();
      }
    });
}

void CStringStorage::Impl::Join()
{
  m_pThread->join();
}

CStringStorage::TResultList& CStringStorage::Impl::GetResults()
{
  return m_results;
}

bool CStringStorage::Impl::PutRawData(PRawData pData, TDataId uid)
{
  if (m_rawLoaded + 1 >= MAX_RAW_LOADED)
    return false;
  m_rawDataList.emplace_back(pData, uid);
  ++m_rawLoaded;
  return true;
}

bool CStringStorage::Impl::GetRawData(PRawData& pData, TDataId& uid)
{
  if (m_rawDataList.empty())
    return false;

  auto elem = m_rawDataList.front();
  pData = elem.first;
  uid = elem.second;
  m_rawDataList.pop_front();
  --m_rawLoaded;
  return true;
}

CStringStorage::TDataId CStringStorage::Impl::CreateResultRecord()
{
  m_results.emplace_back(++m_lineCounter, nullptr);
  if (m_uncompleted == m_results.end())
    m_uncompleted = m_results.begin();
  return m_lineCounter;
}

void CStringStorage::Impl::PutResult(TDataId uid, PResult result)
{
  auto it = std::find_if(m_uncompleted, m_results.end(), [&uid](const auto& elem)
    {
      return elem.first == uid;
    });
  assert(it != m_results.end());
  it->second = result;

  for (auto itFilled = m_uncompleted; itFilled != m_results.end(); ++itFilled)
  {
    if (!itFilled->second)
      break;

    m_uncompleted = itFilled;
  }
}
