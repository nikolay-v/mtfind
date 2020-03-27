#include <algorithm>
#include <fstream>

#include "CSupplierFromFile.h"

struct CSupplierFromFile::Impl
{
  Impl(const std::string& fileName, size_t repeatLen);

  ~Impl();

  std::pair<PRawData, EResult> GetData();

private:
  uint32_t m_bufLen = 4096;
  std::ifstream fin;
  std::unique_ptr<char[]> m_pBuf;
  uint32_t m_line = 1;
  uint32_t m_pos = 0;
  size_t m_repeatLen;
};

CSupplierFromFile::CSupplierFromFile(const std::string& fileName, size_t repeatLen)
  : m_pImpl(std::make_unique<Impl>(fileName, repeatLen)){}

CSupplierFromFile::~CSupplierFromFile() {}

std::pair<PRawData, CSupplierFromFile::EResult> CSupplierFromFile::GetData()
{
  return m_pImpl->GetData();
}

CSupplierFromFile::Impl::Impl(const std::string& fileName, size_t repeatLen): m_repeatLen(repeatLen)
{
  fin.open(fileName);
  if (fin.fail())
    throw 0;
  m_bufLen = std::max(m_bufLen, repeatLen * 10);
  m_pBuf = std::make_unique<char[]>(m_bufLen);
}

CSupplierFromFile::Impl::~Impl()
{
  fin.close();
}

std::pair<PRawData, CSupplierFromFile::EResult> CSupplierFromFile::Impl::GetData()
{
  if (fin.eof())
    return std::make_pair(nullptr, E_EOF);

  auto pData = std::make_shared<TRawData>();
  pData->m_uid = { m_line, m_pos };

  if (m_pos != 0)
    pData->m_data = m_pBuf.get() + (m_bufLen - m_repeatLen);

  fin.clear();
  fin.getline(m_pBuf.get(), m_bufLen);
  if (fin.fail() && *m_pBuf.get() != 0)
  {
    pData->m_data += m_pBuf.get();
    pData->m_eol = false;
    m_pos += m_bufLen - m_repeatLen;
  }
  else
  {
    pData->m_data += m_pBuf.get();
    m_pos = 0;
    ++m_line;
  }

  return std::make_pair(pData, E_OK);
}
