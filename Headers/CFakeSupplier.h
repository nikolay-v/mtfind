#pragma once

#include <list>
#include <string>

#include "DataTypes.h"
#include "IRawDataSupplier.h"

class CFakeSupplier: public IRawDataSupplier<PRawData>
{
public:

	CFakeSupplier()
	{
		static const char* info[] =
		{
			"sddfg",
			"asdasd",
			"sdfsdgg",
			"",
			"???",
			"asd",
			"afdmkcdfgamdard"
		};

		for (auto p : info)
			m_buffer.emplace_back(p);
	}


	std::pair<PRawData, EResult> GetData() override
	{
		if (m_buffer.empty())
			return std::make_pair(nullptr, E_EOF);

		auto pData = std::make_shared<TRawData>();
		pData->m_data = m_buffer.front();
		pData->m_uid.m_lineIndex = m_line++;
		m_buffer.pop_front();

		return std::make_pair(pData, E_OK);
	}

private:

	std::list<std::string> m_buffer;
	uint32_t m_line = 1;
};

