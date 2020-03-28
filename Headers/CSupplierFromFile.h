#pragma once

#include "DataTypes.h"
#include "IRawDataSupplier.h"

class CSupplierFromFile: public IRawDataSupplier<PRawData>
{
public:
	explicit CSupplierFromFile(const std::string& fileName, size_t repeatLen);
	~CSupplierFromFile();

	std::pair<PRawData, EResult> GetData() override;

	struct Impl;
	std::unique_ptr<Impl> m_pImpl;
};
