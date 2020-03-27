#pragma once

class IDataProcessor
{
public:
	virtual ~IDataProcessor() {}

	virtual void Start() = 0;
	virtual void Join() = 0;
};
