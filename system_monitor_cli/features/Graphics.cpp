#include "Graphics.h"

#include <d3d9.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <iostream>

// magic numbers, do not change them
#define NVAPI_MAX_PHYSICAL_GPUS   64
#define NVAPI_MAX_USAGES_PER_GPU  34

// function pointer types
typedef int* (*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int (*NvAPI_Initialize_t)();
typedef int (*NvAPI_EnumPhysicalGPUs_t)(int** handles, int* count);
typedef int (*NvAPI_GPU_GetUsages_t)(int* handle, unsigned int* usages);

std::vector<std::string> Graphics::get_gpu_name()
{
	// Getting GPU name, amount of GPUs, device, 
	LPDIRECT3D9 g_pD3D = NULL;
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		throw;

	D3DADAPTER_IDENTIFIER9 adapter_id;
	g_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapter_id);

	auto gpu_amount = std::to_string(g_pD3D->GetAdapterCount());
	auto gpu_desc = std::string(adapter_id.Description);
	auto gpu_device = std::string(adapter_id.DeviceName);
	auto gpu_driver = std::string(adapter_id.Driver);

	int32_t index = 0;

	// Create the DXGI factory
	IDXGIFactory* pFactory;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
	if (FAILED(hr))
		throw;

	IDXGIAdapter* pAdapter;
	hr = pFactory->EnumAdapters(index, &pAdapter);
	if (FAILED(hr))
		throw;

	DXGI_ADAPTER_DESC desc;

	hr = pAdapter->GetDesc(&desc);
	if (FAILED(hr))
		throw;

	auto gpu_video_memory_bytes = desc.DedicatedVideoMemory;
	auto gpu_system_memory_bytes = desc.DedicatedSystemMemory;
	auto gpu_shared_memory_bytes = desc.SharedSystemMemory;


	std::vector<std::string> result;
	result.push_back(gpu_amount);
	result.push_back(gpu_desc);
	result.push_back(std::to_string(gpu_video_memory_bytes / 1024 / 1024));
	result.push_back(std::to_string(gpu_system_memory_bytes / 1024 / 1024));
	result.push_back(std::to_string(gpu_shared_memory_bytes / 1024 / 1024));
	result.push_back(gpu_driver);
	result.push_back(gpu_device);

	return result;
}

std::vector<std::string> Graphics::get_gpu_usage()
{
	std::vector<std::string> result;

	// Getting VRAM usage
	IDXGIFactory4* pFactory;
	CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory);

	IDXGIAdapter3* adapter;
	pFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&adapter));

	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
	adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);

	size_t usedVRAM = videoMemoryInfo.CurrentUsage;

	result.push_back(std::to_string(usedVRAM));

	// Getting GPU LOAD
	HMODULE hmod = LoadLibraryA("nvapi.dll");
	if (hmod == NULL)
	{
		std::cerr << "Couldn't find nvapi.dll" << std::endl;
		return result;
	}

	// nvapi.dll internal function pointers
	NvAPI_QueryInterface_t      NvAPI_QueryInterface = NULL;
	NvAPI_Initialize_t          NvAPI_Initialize = NULL;
	NvAPI_EnumPhysicalGPUs_t    NvAPI_EnumPhysicalGPUs = NULL;
	NvAPI_GPU_GetUsages_t       NvAPI_GPU_GetUsages = NULL;

	// nvapi_QueryInterface is a function used to retrieve other internal functions in nvapi.dll
	NvAPI_QueryInterface = (NvAPI_QueryInterface_t)GetProcAddress(hmod, "nvapi_QueryInterface");

	// some useful internal functions that aren't exported by nvapi.dll
	NvAPI_Initialize = (NvAPI_Initialize_t)(*NvAPI_QueryInterface)(0x0150E828);
	NvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t)(*NvAPI_QueryInterface)(0xE5AC921F);
	NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t)(*NvAPI_QueryInterface)(0x189A1FDF);

	if (NvAPI_Initialize == NULL || NvAPI_EnumPhysicalGPUs == NULL ||
		NvAPI_EnumPhysicalGPUs == NULL || NvAPI_GPU_GetUsages == NULL)
	{
		std::cerr << "Couldn't get functions in nvapi.dll" << std::endl;
		return result;
	}

	// initialize NvAPI library, call it once before calling any other NvAPI functions
	(*NvAPI_Initialize)();

	int          gpuCount = 0;
	int* gpuHandles[NVAPI_MAX_PHYSICAL_GPUS] = { NULL };
	unsigned int gpuUsages[NVAPI_MAX_USAGES_PER_GPU] = { 0 };

	// gpuUsages[0] must be this value, otherwise NvAPI_GPU_GetUsages won't work
	gpuUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;

	(*NvAPI_EnumPhysicalGPUs)(gpuHandles, &gpuCount);
	(*NvAPI_GPU_GetUsages)(gpuHandles[0], gpuUsages);
	int usage = gpuUsages[3];
	result.push_back(std::to_string(usage));
	return result;
}
