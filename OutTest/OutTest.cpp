// OutTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <Windows.h>
#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <functiondiscoverykeys_devpkey.h>
#include <comdef.h>

#include <wchar.h>
#include <wrl/client.h>
#include <locale> // 包含 <codecvt>
#pragma comment(lib, "Ole32.lib")

using namespace Microsoft::WRL;

// 获取并打印所有音频设备的设备ID、设备名和静音状态
void PrintAudioDevices() {
    CoInitialize(NULL); // 初始化COM库
    
    IMMDeviceEnumerator* pEnumerator = NULL;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        wprintf(L"创建设备枚举器失败\n");
        CoUninitialize();
        return;
    }

    IMMDeviceCollection* pCollection = NULL;
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr)) {
        wprintf(L"获取音频设备集合失败\n");
        pEnumerator->Release();
        CoUninitialize();
        return;
    }

    UINT count;
    pCollection->GetCount(&count);
    for (UINT i = 0; i < count; ++i) {
        IMMDevice* pDevice = NULL;
        hr = pCollection->Item(i, &pDevice);
        if (SUCCEEDED(hr)) {
            LPWSTR deviceId = NULL;
            hr = pDevice->GetId(&deviceId);
            if (SUCCEEDED(hr)) {
                wprintf(L"deviceId===%s\n", deviceId);
                CoTaskMemFree(deviceId);
            }

            IPropertyStore* pProps = NULL;
            hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
            if (SUCCEEDED(hr)) {
                PROPVARIANT varName;
                PropVariantInit(&varName);
                hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
                if (SUCCEEDED(hr)) {
                    wprintf(L"varName===%s\n", varName.pwszVal);
                    PropVariantClear(&varName);
                }
                pProps->Release();
            }

            IAudioEndpointVolume* pVol = NULL;
            hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&pVol);
            if (SUCCEEDED(hr)) {
                BOOL mute;
                hr = pVol->GetMute(&mute);
                if (SUCCEEDED(hr)) {
                    wprintf(L"mute===%d\n", mute);
                }
                pVol->Release();
            }

            pDevice->Release();
        }
    }

    pCollection->Release();
    pEnumerator->Release();
    CoUninitialize();
}

// 获取并打印所有音频设备的设备ID、设备名和静音状态，并判断是否为当前音频输出设备
void PrintAudioDevices_A() {
    CoInitializeEx(NULL, COINIT_MULTITHREADED); // 初始化COM库

    ComPtr<IMMDeviceEnumerator> pEnumerator;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        wprintf(L"创建设备枚举器失败\n");
        CoUninitialize();
        return;
    }

    ComPtr<IMMDeviceCollection> pCollection;
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr)) {
        wprintf(L"创建设备枚举器失败\n");
        CoUninitialize();
        return;
    }

    UINT count;
    pCollection->GetCount(&count);
    for (UINT i = 0; i < count; ++i) {
        ComPtr<IMMDevice> pDevice;
        hr = pCollection->Item(i, &pDevice);
        if (SUCCEEDED(hr)) {
            LPWSTR deviceId = NULL;
            hr = pDevice->GetId(&deviceId);
            if (SUCCEEDED(hr)) {
                wprintf(L"deviceId===%s\n", deviceId);
                CoTaskMemFree(deviceId);
            }

            ComPtr<IPropertyStore> pProps;
            hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
            if (SUCCEEDED(hr)) {
                PROPVARIANT varName;
                PropVariantInit(&varName);
                hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
                if (SUCCEEDED(hr)) {
                    wprintf(L"varName===%s\n", varName.pwszVal);
                    PropVariantClear(&varName);
                }
            }

            ComPtr<IAudioEndpointVolume> pVol;
            hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&pVol);
            if (SUCCEEDED(hr)) {
                BOOL mute;
                hr = pVol->GetMute(&mute);
                if (SUCCEEDED(hr)) {
                    wprintf(L"mute===%d\n", mute);
                }
            }

            // 判断是否为当前音频输出设备
            ComPtr<IMMDevice> pDefaultDevice;
            hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDefaultDevice);
            if (SUCCEEDED(hr)) {
                LPWSTR defaultDeviceId = NULL;
                hr = pDefaultDevice->GetId(&defaultDeviceId);
                if (SUCCEEDED(hr)) {
                    wprintf(L"wcscmp===%d\n", wcscmp(deviceId, defaultDeviceId));
                    CoTaskMemFree(defaultDeviceId);
                }
            }
        }
    }

    CoUninitialize();
}

// 获取并打印所有音频设备的设备ID、设备名和静音状态，并判断是否为当前音频输出设备
void PrintAudioDevices_B() {
    CoInitializeEx(NULL, COINIT_MULTITHREADED); // 初始化COM库

    ComPtr<IMMDeviceEnumerator> pEnumerator;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        std::wcerr << L"创建设备枚举器失败" << std::endl;
        CoUninitialize();
        return;
    }

    ComPtr<IMMDeviceCollection> pCollection;
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr)) {
        std::wcerr << L"获取音频设备集合失败" << std::endl;
        CoUninitialize();
        return;
    }

    ComPtr<IMMDevice> pDefaultDevice;
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDefaultDevice);
    if (FAILED(hr)) {
        std::wcerr << L"获取默认音频输出设备失败" << std::endl;
        CoUninitialize();
        return;
    }

    LPWSTR defaultDeviceId = NULL;
    hr = pDefaultDevice->GetId(&defaultDeviceId);
    if (FAILED(hr)) {
        std::wcerr << L"获取默认音频输出设备ID失败" << std::endl;
        CoUninitialize();
        return;
    }

    UINT count;
    pCollection->GetCount(&count);
    for (UINT i = 0; i < count; ++i) {
        ComPtr<IMMDevice> pDevice;
        hr = pCollection->Item(i, &pDevice);
        if (SUCCEEDED(hr)) {
            LPWSTR deviceId = NULL;
            hr = pDevice->GetId(&deviceId);
            if (SUCCEEDED(hr)) {
                std::wcout << L"deviceId: " << deviceId << std::endl;

                // 判断是否为当前音频输出设备
                std::wcout << L"当前设备是否为系统音频输出设备: " << (wcscmp(deviceId, defaultDeviceId) == 0 ? L"是" : L"否") << std::endl;

                CoTaskMemFree(deviceId);
            }

            ComPtr<IPropertyStore> pProps;
            hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
            if (SUCCEEDED(hr)) {
                PROPVARIANT varName;
                PropVariantInit(&varName);
                hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
                if (SUCCEEDED(hr)) {
                    std::wcout << L"设备名: " << varName.pwszVal << std::endl;
                    PropVariantClear(&varName);
                }
            }

            ComPtr<IAudioEndpointVolume> pVol;
            hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&pVol);
            if (SUCCEEDED(hr)) {
                BOOL mute;
                hr = pVol->GetMute(&mute);
                if (SUCCEEDED(hr)) {
                    std::wcout << L"静音状态: " << (mute ? L"已静音" : L"未静音") << std::endl;
                }
            }
        }
    }

    CoTaskMemFree(defaultDeviceId);
    CoUninitialize();
}

// 获取并打印所有音频设备的设备ID、设备名和静音状态，并判断是否为当前音频输出设备
void PrintAudioDevices_C() {
    CoInitializeEx(NULL, COINIT_MULTITHREADED); // 初始化COM库

    ComPtr<IMMDeviceEnumerator> pEnumerator;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        wprintf(L"创建设备枚举器失败\n");
        CoUninitialize();
        return;
    }

    ComPtr<IMMDeviceCollection> pCollection;
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr)) {
        wprintf(L"获取音频设备集合失败\n");
        CoUninitialize();
        return;
    }

    ComPtr<IMMDevice> pDefaultDevice;
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDefaultDevice);
    if (FAILED(hr)) {
        wprintf(L"获取默认音频输出设备失败\n");
        CoUninitialize();
        return;
    }

    LPWSTR defaultDeviceId = NULL;
    hr = pDefaultDevice->GetId(&defaultDeviceId);
    if (FAILED(hr)) {
        wprintf(L"获取默认音频输出设备ID失败\n");
        CoUninitialize();
        return;
    }

    UINT count;
    pCollection->GetCount(&count);
    for (UINT i = 0; i < count; ++i) {
        ComPtr<IMMDevice> pDevice;
        hr = pCollection->Item(i, &pDevice);
        if (SUCCEEDED(hr)) {
            LPWSTR deviceId = NULL;
            hr = pDevice->GetId(&deviceId);
            if (SUCCEEDED(hr)) {
                wprintf(L"deviceId: %s\n", deviceId);

                // 判断是否为当前音频输出设备
                wprintf(L"wcscmp: %d\n", wcscmp(deviceId, defaultDeviceId));

                CoTaskMemFree(deviceId);
            }

            ComPtr<IPropertyStore> pProps;
            hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
            if (SUCCEEDED(hr)) {
                PROPVARIANT varName;
                PropVariantInit(&varName);
                hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);
                if (SUCCEEDED(hr)) {
                    wprintf(L"varName: %s\n", varName.pwszVal);
                    PropVariantClear(&varName);
                }
            }

            ComPtr<IAudioEndpointVolume> pVol;
            hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&pVol);
            if (SUCCEEDED(hr)) {
                BOOL mute;
                hr = pVol->GetMute(&mute);
                if (SUCCEEDED(hr)) {
                    wprintf(L"mute: %d\n", mute);
                }
            }
        }
    }

    CoTaskMemFree(defaultDeviceId);
    CoUninitialize();
}


int main()
{
    //PrintAudioDevices();
    //PrintAudioDevices_A();
    //PrintAudioDevices_B();
    //PrintAudioDevices_C();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
