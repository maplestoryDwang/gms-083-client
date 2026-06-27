#include "pch.h"
#include "hook.h"
#include "debug.h"
#include <windows.h>
#include <detours.h>
#include <psapi.h>


namespace {

bool HexCharToByte(char c, unsigned char* b) {
    if ('0' <= c && c <= '9')
        *b = c - '0';
    else if ('A' <= c && c <= 'F')
        *b = 10 + (c - 'A');
    else if ('a' <= c && c <= 'f')
        *b = 10 + (c - 'a');
    else
        return false;
    return true;
}

/*
作用：辅助函数。将形如 "55 8B EC ?? ?? 83" 的特征码字符串，解析为字节数组和掩码（Mask）数组。
工作原理：遇到正常的十六进制数（如 55），就转为字节存入 abPattern，并在 abMask 对应位置设为 0xFF（表示必须精确匹配）；
遇到 ?? 通配符，则在 abMask 对应位置设为 0x00（表示任意字节都可以匹配）。
*/
size_t ParsePattern(const char* sPattern, unsigned char* abPattern, unsigned char* abMask) {
    size_t i = 0;
    while (*sPattern) {
        if (*sPattern == ' ') {
            sPattern++;
            continue;
        }
        if (sPattern[0] == '?' && sPattern[1] == '?') {
            abMask[i] = 0x00;
        } else {
            unsigned char high, low;
            if (!HexCharToByte(sPattern[0], &high) || !HexCharToByte(sPattern[1], &low)) {
                return 0;
            }
            abPattern[i] = (high << 4) | low;
            abMask[i] = 0xFF;
        }
        sPattern += 2;
        i += 1;
    }
    return i;
}

void* FindPattern(unsigned char* pModuleBase, size_t uModuleSize, unsigned char* abPattern, unsigned char* abMask, size_t uPatternSize) {
    if (uModuleSize < uPatternSize) {
        return nullptr;
    }
    for (size_t i = 0; i <= uModuleSize - uPatternSize; ++i) {
        size_t j;
        for (j = 0; j < uPatternSize; ++j) {
            if ((pModuleBase[i + j] & abMask[j]) != (abPattern[j] & abMask[j])) {
                break;
            }
        }
        if (j == uPatternSize) {
            return &pModuleBase[i];
        }
    }
    return nullptr;
}

} // namespace

/*
作用：利用微软的 Detours 库，对一个普通的 API 或函数进行标准的 Inline Hook（内联挂钩）。

工作原理：
DetourTransactionBegin()：开启一个 Hook 事务。
DetourUpdateThread()：将当前线程加入事务，确保多线程环境下修改内存时的线程安全。
DetourAttach(ppTarget, pDetour)：核心步骤。它会修改 ppTarget 指向的函数开头，写入一个跳转指令（JMP）到你的自定义函数 pDetour，
同时把原函数的备份地址存回 ppTarget（以便你后续还能调用原函数）。
DetourTransactionCommit()：提交并生效这次修改。
*/
bool AttachHook(void** ppTarget, void* pDetour) {
    LONG result;
    if (result = DetourTransactionBegin(); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourTransactionBegin failed with : %d", result);
        return false;
    }
    if (result = DetourUpdateThread(GetCurrentThread()); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourUpdateThread failed with : %d", result);
        DetourTransactionAbort();
        return false;
    }
    if (result = DetourAttach(ppTarget, pDetour); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourAttach failed with : %d", result);
        DetourTransactionAbort();
        return false;
    }
    if (result = DetourTransactionCommit(); result != NO_ERROR) {
        DEBUG_MESSAGE("DetourTransactionCommit failed with : %d", result);
        DetourTransactionAbort();
        return false;
    }
    return true;
}
/*
作用：对 C++ 虚函数表（Virtual Method Table, VMT）中的某个虚函数进行 Hook。

工作原理：
*static_cast<void***>(pInstance)：获取某个 C++ 类对象的虚函数表指针（vtable）。
vtable[uIndex]：根据索引 uIndex 找到具体的虚函数地址 pTarget。
随后调用前面的 AttachHook，对这个具体的虚函数地址进行拦截，并返回原函数的地址。

*/
void* VMTHook(void* pInstance, void* pDetour, size_t uIndex) {
    void** vtable = *static_cast<void***>(pInstance);
    void* pTarget = vtable[uIndex];
    AttachHook(&pTarget, pDetour);
    return pTarget;
}
/*
作用：通过模块名和导出函数名，动态获取某个 DLL 中的函数地址。

工作原理：先尝试用 GetModuleHandleA 获取已加载模块的句柄，如果没有加载，则用 LoadLibraryA 强制加载它。最后通过 Windows API GetProcAddress 拿到函数的真实内存地址。
*/
void* GetAddress(const char* sModuleName, const char* sProcName) {
    HMODULE hModule = GetModuleHandleA(sModuleName);
    if (!hModule) {
        hModule = LoadLibraryA(sModuleName);
        if (!hModule) {
            ErrorMessage("Could not load library %s with error %d", sModuleName, GetLastError());
            return nullptr;
        }
    }
    FARPROC result = GetProcAddress(hModule, sProcName);
    if (!result) {
        ErrorMessage("Could not resolve address for %s in module %s", sProcName, sModuleName);
    }
    return reinterpret_cast<void*>(result);
}
/*
* 作用：特征码搜索（Pattern Scanning）。在指定的模块内存中，暴力搜索匹配特定机器码指令的地址。

工作原理：
通过 GetModuleInformation 获取目标模块在内存中的起始基址（lpBaseOfDll）和总大小（SizeOfImage）。
调用 ParsePattern 解析特征码。
使用双重循环在整个模块的内存中进行滑动比对。比对时利用 & abMask 过滤掉通配符（??）。
一旦匹配成功，立刻返回该段内存的绝对地址。

*/
void* GetAddressByPattern(const char* sModuleName, const char* sPattern) {
    HMODULE hModule = GetModuleHandleA(sModuleName);
    if (!hModule) {
        hModule = LoadLibraryA(sModuleName);
        if (!hModule) {
            ErrorMessage("Could not load library %s with error %d", sModuleName, GetLastError());
            return nullptr;
        }
    }
    MODULEINFO mi;
    if (!GetModuleInformation(GetCurrentProcess(), hModule, &mi, sizeof(mi))) {
        ErrorMessage("Could not get module information for : %s", sModuleName);
        return nullptr;
    }
    unsigned char* pModuleBase = static_cast<unsigned char*>(mi.lpBaseOfDll);
    size_t uModuleSize = mi.SizeOfImage;

    unsigned char abPattern[1024];
    unsigned char abMask[1024];
    size_t uPatternSize = ParsePattern(sPattern, abPattern, abMask);
    if (uPatternSize == 0) {
        ErrorMessage("Could not parse pattern : %s", sPattern);
        return nullptr;
    }

    void* pAddress = FindPattern(pModuleBase, uModuleSize, abPattern, abMask, uPatternSize);
    if (!pAddress) {
        ErrorMessage("Could not resolve address for pattern \"%s\" in module %s", sPattern, sModuleName);
    }
    return pAddress;
}

/*
工作原理：因为代码段通常是只读的（PAGE_EXECUTE_READ），直接写入会引发崩溃。
所以先用 VirtualProtect 改为可读可写可执行（PAGE_EXECUTE_READWRITE），写入数据后，再恢复原本的内存保护属性。
*/
void PatchMemory(void* pAddress, void* pValue, size_t uSize) {
    DWORD flOldProtect;
    VirtualProtect(pAddress, uSize, PAGE_EXECUTE_READWRITE, &flOldProtect);
    memcpy(pAddress, pValue, uSize);
    VirtualProtect(pAddress, uSize, flOldProtect, &flOldProtect);
}

void PatchAllByPattern(void* pStart, void* pEnd, const char* sPattern, void* pValue, size_t uSize) {
    unsigned char abPattern[1024];
    unsigned char abMask[1024];
    size_t uPatternSize = ParsePattern(sPattern, abPattern, abMask);
    if (uPatternSize == 0) {
        ErrorMessage("Could not parse pattern : %s", sPattern);
        return;
    }

    unsigned char* pCurrent = static_cast<unsigned char*>(pStart);
    while (pCurrent < pEnd) {
        size_t uRemainSize = reinterpret_cast<uintptr_t>(pEnd) - reinterpret_cast<uintptr_t>(pCurrent);
        void* pTarget = FindPattern(pCurrent, uRemainSize, abPattern, abMask, uPatternSize);
        if (!pTarget) {
            break;
        }
        PatchMemory(pTarget, pValue, uSize);
        pCurrent += uSize;
    }
}
