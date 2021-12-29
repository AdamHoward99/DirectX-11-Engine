#include "AdapterReader.h"

Data::Data(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&pAdapterDescription);
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, 6, "AdapterReader.cpp","IDXGIAdapter::GetDesc()");
}

AdapterReader::AdapterReader()
{}

std::vector<Data> AdapterReader::GetData()
{
	//Adapters are already initialised
	if (adapters.size() > 0)
		return adapters;

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	//Create DXGIFactory Obj
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());
	if (FAILED(hr))
		ErrorMes::DisplayHRErrorMessage(hr, 23, "AdapterReader.cpp", "CreateDXGIFactory()");

	IDXGIAdapter* pAdapter;

	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		adapters.push_back(Data(pAdapter));
		index++;
	}

	return adapters;
}
