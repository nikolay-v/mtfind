#include <array>
#include <iostream>

#include "CStringProcessor.h"
#include "CStringStorage.h"
#include "CSubStringSearch.h"
#include "CSupplierFromFile.h"


static const size_t PROC_THREADS_COUNT = 1;

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "wrong argument list (" << argc << ")" << std::endl;
    return -1;
  }

  std::string fileName = argv[1];
  std::string searchMask = argv[2];
  std::shared_ptr<IRawDataSupplier<PRawData>> pSupplier;
  try
  {
    pSupplier = std::make_shared<CSupplierFromFile>(fileName, searchMask.length());
  }
  catch (...)
  {
    std::cout << "can't read file" << std::endl;
    return -1;
  }
  auto pStorage = std::make_shared<CStringStorage>(pSupplier);
  auto pSeacher = std::make_shared<CSubStringSearch>();
  std::array<std::shared_ptr<CStringProcessor>, PROC_THREADS_COUNT> processors;

  for (auto& proc : processors)
    proc = std::make_shared<CStringProcessor>(pStorage, pSeacher, searchMask);

  pStorage->Start();
  for (auto& proc : processors)
    proc->Start();

  pStorage->Join();
  for (auto& proc : processors)
    proc->Join();

  auto seachResults = pStorage->GetResults();

  size_t count = 0;
  for (auto record : seachResults)
  {
    if (record.second.get() == nullptr || record.second->m_results.empty())
      continue;
    count += record.second->m_results.size();
  }

  std::cout << count << std::endl;

  for (auto record : seachResults)
  {
    if (record.second.get() == nullptr)
      continue;

    for (auto pos : record.second->m_results)
      std::cout << record.second->m_line << " " << pos.m_pos << " " << pos.m_subString << std::endl;
  }

  return 0;
}