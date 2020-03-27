#pragma once

#include <stdint.h>

#include <list>
#include <memory>
#include <string>
//#include <vector>

struct TUid
{
  uint32_t m_lineIndex;
  uint32_t m_pos;
};

struct TRawData
{
  //std::vector<char> m_data;
  std::string m_data;
  TUid m_uid = {0, 0};
  bool m_eol = true;
};
using PRawData = std::shared_ptr<TRawData>;

struct TResult
{
  struct TLikeness
  {
    uint32_t m_pos;
    std::string m_subString;
  };

  uint32_t m_line;
  std::list<TLikeness> m_results;
};
using PResult = std::shared_ptr<TResult>;
