#pragma once

#include <list>
#include <memory>
#include <stdint.h>
#include <utility>

template <typename PData, typename PProcessResult> class IDataStorage
{
public:

	enum EResult
	{
		E_OK,
		E_NO_DATA,
		E_END_OF_DATA
	};

	using TDataId = size_t;
	struct TDataInfo
	{
		TDataInfo(EResult result, TDataId id) : m_result(result), m_id(id) {}
		EResult				 m_result;
		TDataId				 m_id;
	};
	using PDataInfo = std::shared_ptr<TDataInfo>;

	using TResultList = std::list<std::pair<TDataId, PProcessResult>>;
	
	virtual ~IDataStorage() {}

	virtual std::pair<PData, PDataInfo> GetData() = 0;
	virtual void ReturnResult(PDataInfo, PProcessResult) = 0;

	virtual void Start() = 0;
	virtual void Join() = 0;

	virtual TResultList& GetResults() = 0;
};
