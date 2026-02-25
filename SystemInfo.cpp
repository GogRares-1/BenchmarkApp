
#include "SystemInfo.hpp"

#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

string getCPUBrand() {
    int cpuInfo[4];
    char brand[50];

    __cpuid(cpuInfo, 0x80000002);
    memcpy(brand, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000003);
    memcpy(brand + 16, cpuInfo, sizeof(cpuInfo));

    __cpuid(cpuInfo, 0x80000004);
    memcpy(brand + 32, cpuInfo, sizeof(cpuInfo));

    brand[48] = '\0';
    return string(brand);
}

struct GPUInfo {
    string name;
    double ramGB;
};

vector<GPUInfo> getAllGPUs() {
    vector<GPUInfo> gpus;

    DISPLAY_DEVICE dd;
    dd.cb = sizeof(dd);

    for (int i = 0; ; i++) {
        if (!EnumDisplayDevices(NULL, i, &dd, EDD_GET_DEVICE_INTERFACE_NAME))
            break;

        wstring wname(dd.DeviceString);
        string gpuName(wname.begin(), wname.end());

        bool exists = false;
        for (auto& g : gpus) {
            if (g.name == gpuName) {
                exists = true;
                break;
            }
        }
        if (exists)
            continue;

        gpus.push_back({ gpuName, 0.0 });
    }

    return gpus;
}

double getSystemRAM_GB() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);

    if (GlobalMemoryStatusEx(&memInfo)) {
        DWORDLONG totalBytes = memInfo.ullTotalPhys;
        return totalBytes / (1024.0 * 1024.0 * 1024.0);
    }

    return 0.0;
}

void getCPUCoreInfo(int& physicalCores, int& logicalProcessors) {
    physicalCores = 0;
    logicalProcessors = 0;

    DWORD length = 0;
    GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &length);

    vector<char> buffer(length);
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)buffer.data();

    GetLogicalProcessorInformationEx(RelationProcessorCore, info, &length);

    char* ptr = buffer.data();
    char* end = ptr + length;

    while (ptr < end) {
        PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX item = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)ptr;

        if (item->Relationship == RelationProcessorCore) {
            physicalCores++;
            for (WORD g = 0; g < item->Processor.GroupCount; g++) {
                logicalProcessors += __popcnt(item->Processor.GroupMask[g].Mask);
            }
        }
        ptr += item->Size;
    }
}

double getCPUBaseSpeedGHz() {
    HKEY hKey;
    DWORD speedMHz = 0;
    DWORD size = sizeof(DWORD);

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",0,KEY_READ,&hKey) == ERROR_SUCCESS) 
    {
        RegQueryValueEx(hKey,L"~MHz",nullptr,nullptr,(LPBYTE)&speedMHz,&size);
        RegCloseKey(hKey);
    }
    return speedMHz / 1000.0;
}

void printSystemInfo() {
    cout << "\n=== SYSTEM INFORMATION ===\n\n";

    // CPU
    cout << "CPU: " << getCPUBrand() << "\n";

    int physicalCores = 0;
    int logicalProcessors = 0;
    getCPUCoreInfo(physicalCores, logicalProcessors);

    cout << "Cores: " << physicalCores << "\n";
    cout << "Logical processors: " << logicalProcessors << "\n";

    double baseSpeed = getCPUBaseSpeedGHz();
    cout << "Base speed: " << baseSpeed << " GHz\n\n";

    // GPU
    vector<GPUInfo> gpus = getAllGPUs();
    for (int i = 0; i < gpus.size(); i++) {
        cout << "GPU #" << i << ": " << gpus[i].name << "\n";
    }

    // MEMORY
    double ramGB = getSystemRAM_GB();
    cout << "\nMemory RAM: " << ramGB << " GB\n";
}