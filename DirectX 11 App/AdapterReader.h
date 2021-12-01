#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "ErrorMessaging.h"

class Data
{
public:
	Data(IDXGIAdapter*);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC pAdapterDescription;
};

class AdapterReader
{
public:
	AdapterReader();
	std::vector<Data> GetData();

private:
	std::vector<Data> adapters;
};