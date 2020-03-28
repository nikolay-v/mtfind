#pragma once

#include <memory>
#include <utility>

template <typename PData> class IRawDataSupplier
{
public:
	enum EResult
	{
		E_OK,
		E_EOF,
		E_ERROR
	};

	virtual ~IRawDataSupplier() {}

	virtual std::pair<PData, EResult> GetData() = 0;
};

template<class T> using PRawDataSupplier = std::shared_ptr<IRawDataSupplier<T>>;
