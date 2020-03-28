#include "CSubStringSearch.h"

static const char ANY_SIGN = '?';

PResult CSubStringSearch::Search(const std::string& pData, const std::string& pMask)
{
  auto result = std::make_shared<TResult>();

  size_t maskLen = pMask.length();
  size_t dataLen = pData.length();
  if (dataLen < maskLen || maskLen == 0)
    return result;

  for (size_t i = 0; i < dataLen - maskLen + 1; ++i)
  {
    size_t j = 0;
    for (; j < maskLen; ++j)
    {
      if (pMask[j] != ANY_SIGN && pMask[j] != pData[i + j])
        break;
    }
    if (j == maskLen)
    {
      result->m_results.push_back(TResult::TLikeness({ i + 1, pData.substr(i, maskLen) }));
      i += j - 1;
    }
  }

  return result;
}
