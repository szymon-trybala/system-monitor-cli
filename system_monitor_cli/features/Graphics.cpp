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
	try {
		std::vector<std::string> result;

		// Getting GPU name, amount of GPUs, device, 
		LPDIRECT3D9 g_pD3D = NULL;
		if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
			throw;

		D3DADAPTER_IDENTIFIER9 adapter_id;
		g_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapter_id);

		auto gpu_desc = std::string(adapter_id.Description);
		auto gpu_device = std::string(adapter_id.DeviceName);
		auto gpu_driver = std::string(adapter_id.Driver);

		result.push_back(gpu_desc);
		result.push_back(gpu_driver);
		result.push_back(gpu_device);

		// Getting VRAM amount
		IDXGIFactory4* pFactory;
		if (CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void**)&pFactory) == S_OK)
		{
			IDXGIAdapter3* adapter;
			pFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(&adapter));

			DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
			adapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
			auto gpu_vram = videoMemoryInfo.Budget;

			auto ceiled = gpu_vram / 1000 / 1000;
			auto calced = (int)ceiled / 1000;
			if ((int)ceiled % 1000 > 0)
				calced++;

			result.push_back(std::to_string(calced * 1024));
		}
		return result;
	}
	catch (...) {
		std::cerr << "Exception was thrown during getting GPU name, program will exit" << std::endl;
		exit(-1);
	}
}

std::vector<std::string> Graphics::get_gpu_usage()
{
	try {
		std::vector<std::string> result;

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
	catch (...) {
		std::cerr << "Exception was thrown during getting GPU usage, program will exit" << std::endl;
		exit(-1);
	}
}
