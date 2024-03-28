#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <functiondiscoverykeys_devpkey.h>

#include <wrl/client.h>
using namespace Microsoft::WRL;

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
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATEMASK_ALL, &pCollection);
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
        wprintf(L"----------------------------------------------------------------\n");
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

void PrintAudioDevices_D() {
    CoInitializeEx(NULL, COINIT_MULTITHREADED); // 初始化COM库

    ComPtr<IMMDeviceEnumerator> pEnumerator;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        wprintf(L"创建设备枚举器失败\n");
        CoUninitialize();
        return;
    }

    ComPtr<IMMDeviceCollection> pCollection;
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATEMASK_ALL, &pCollection);
    if (FAILED(hr)) {
        wprintf(L"获取音频设备集合失败\n");
        CoUninitialize();
        return;
    }

    UINT count;
    pCollection->GetCount(&count);
    for (UINT i = 0; i < count; ++i) {
        wprintf(L"----------------------------------------------------------------\n");
        ComPtr<IMMDevice> pDevice;
        hr = pCollection->Item(i, &pDevice);
        if (SUCCEEDED(hr)) {
            ComPtr<IAudioEndpointVolume> pVol;
            hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&pVol);
            if (SUCCEEDED(hr)) {
                BOOL mute;
                hr = pVol->GetMute(&mute);
                if (SUCCEEDED(hr)) {
                    wprintf(L"mute: %d\n", mute);
                    if (!mute) {
                        // 如果设备没有静音，则设置静音
                        hr = pVol->SetMute(TRUE, NULL);
                        if (SUCCEEDED(hr)) {
                            wprintf(L"设备已设置为静音\n");
                        }
                        else {
                            wprintf(L"设置静音失败\n");
                        }
                    }
                }
            }
        }
    }

    CoUninitialize();
}

int main()
{
    PrintAudioDevices_D();
    return 0;
}