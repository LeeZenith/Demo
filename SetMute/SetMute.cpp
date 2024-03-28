// SetMute.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <functiondiscoverykeys_devpkey.h>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Mmdevapi.lib")

#include <iostream>
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
                            wprintf(L"SetMute\n");
                        }
                        else {
                            wprintf(L"SetMute Fail\n");
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
    PrintAudioDevices_C();
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
