#pragma once
#include "debug.h"
#include <type_traits>

/*
* 自动化函数挂钩宏
reinterpret_cast<void>(&TARGET)：自动把你的游戏原版函数指针强转为 void，省去了你每次手动写强转的麻烦。
CastHook(&DETOUR)：将你的 Hook 目标函数转为通用的 void*（下面会详细讲 CastHook 的特殊之处）。
#DETOUR（仅在 _DEBUG 下）：C++ 预处理器的特性，能把你的函数名直接转换成字符串。
如果 Hook 失败，它会弹出一行极其人性化的报错：“Failed to attach detour function 'CWvsApp__ctor_hook' at target address: 0x009CA8A0”。
*/
#ifdef _DEBUG
#define ATTACH_HOOK(TARGET, DETOUR) \
    AttachHook(reinterpret_cast<void**>(&TARGET), CastHook(&DETOUR)) ? true : (ErrorMessage("Failed to attach detour function \"%s\" at target address : 0x%08X.", #DETOUR, TARGET), false)
#else
#define ATTACH_HOOK(TARGET, DETOUR) \
    AttachHook(reinterpret_cast<void**>(&TARGET), CastHook(&DETOUR))
#endif


/*
MEMBER_AT(T, OFFSET, NAME) —— 极其硬核的“内存偏移隐式映射”宏
这是整个头文件里技术含量最高的宏。它是为了解决 C++ 逆向工程中一个极其头疼的问题：
“我知道游戏某个类在某个偏移量（Offset）有个变量，但我没办法重构整个类的结构体，该怎么读写它？”
它利用了微软 VC++ 编译器的独有特性 __declspec(property)，在类的内部虚拟出一个成员变量。

__declspec(property(get = get_##NAME, put = set_##NAME)) T NAME;
声明一个名为 NAME、类型为 T 的虚拟属性。当你代码里写 obj.NAME 时，编译器会自动去调用 get_NAME()；写 obj.NAME = val 时，自动调用 set_NAME()。

get_##NAME() const 与 get_##NAME()
利用 reinterpret_cast<uintptr_t>(this) + OFFSET，直接拿当前对象的首地址（this 指针），硬生生加上你指定的字节偏移量，然后把那块内存强转成类型 T 的引用返回。

set_##NAME(...)
同理，找到偏移后的内存，直接往里写入新值。


实际应用场景：
假设原版游戏里的类 CWvsApp 大小有几个 KB，你想修改偏移量为 0x203C 处的 m_bWindowActive（窗口是否激活，布尔值）。
你不需要在 C++ 里写一堆占位数组，直接在类定义里塞入这个宏：

class CWvsApp {
public:
    MEMBER_AT(bool, 0x203C, m_bWindowActive);
};
在后续代码中，你可以像对待原生成员一样极其自然地操作它：

pThis->m_bWindowActive = true; // 编译器在底层会自动将其转化为：*(bool*)((char*)pThis + 0x203C) = true;


*/
#define MEMBER_AT(T, OFFSET, NAME) \
    __declspec(property(get = get_##NAME, put = set_##NAME)) T NAME; \
    __forceinline const T& get_##NAME() const { \
        return *reinterpret_cast<const T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    } \
    __forceinline T& get_##NAME() { \
        return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    } \
    __forceinline void set_##NAME(const T& value) { \
        *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET) = const_cast<T&>(value); \
    } \
    __forceinline void set_##NAME(T& value) { \
        *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + OFFSET) = value; \
    }

#define MEMBER_ARRAY_AT(T, OFFSET, NAME, N) \
    __declspec(property(get = get_##NAME)) T(&NAME)[N]; \
    __forceinline T(&get_##NAME())[N] { \
        return *reinterpret_cast<T(*)[N]>(reinterpret_cast<uintptr_t>(this) + OFFSET); \
    }

#define MEMBER_HOOK(T, ADDRESS, NAME, ...) \
    inline static auto NAME = reinterpret_cast<T(__thiscall*)(void*, __VA_ARGS__)>(ADDRESS); \
    T NAME##_hook(__VA_ARGS__);

#define TO_UINTPTR(VALUE) ((uintptr_t)(VALUE))

#define TO_PVOID(VALUE) ((void*)(VALUE))


// called in injector.cpp -> DllMain
void AttachSystemHooks();

// called in system.cpp -> SetUnhandledExceptionFilter_hook
void AttachClientBypass();
void AttachClientInlink();
void AttachStringPoolMod();
void AttachResManMod();
void AttachAvatarDataMod();
void AttachItemEffectMod();
void AttachResolutionMod();
void AttachMobHpTagMod();
void AttachToolTipMod();
void AttachIconIconMod();
void AttachTempStatMod();
void AttachQuickSlotsMod(); // <-- add this line


inline void AttachClientHooks() {
    AttachClientBypass();
    AttachClientInlink();
    AttachStringPoolMod();
    AttachResManMod();       // 加载WZ
    AttachAvatarDataMod();
    AttachItemEffectMod();
    AttachResolutionMod();
    AttachMobHpTagMod();
    AttachToolTipMod();
    AttachIconIconMod();
    AttachTempStatMod();
    AttachQuickSlotsMod(); // <-- add this line
}


template <typename T>
constexpr auto CastHook(T fn) -> void* {
    union {
        T fn;
        void* p;
    } u;
    u.fn = fn;
    return u.p;
}

bool AttachHook(void** ppTarget, void* pDetour);

void* VMTHook(void* pInstance, void* pDetour, size_t uIndex);

void* GetAddress(const char* sModuleName, const char* sProcName);

void* GetAddressByPattern(const char* sModuleName, const char* sPattern);

void PatchMemory(void* pAddress, void* pValue, size_t uSize);

char* EncodeStringAlloc(const char* sSource); // <-- add this line


void PatchAllByPattern(void* pStart, void* pEnd, const char* sPattern, void* pValue, size_t uSize);


template <typename T>
void Patch1(T pAddress, unsigned char uValue) {
    PatchMemory(TO_PVOID(pAddress), &uValue, sizeof(uValue));
}

template <typename T>
void Patch4(T pAddress, unsigned int uValue) {
    PatchMemory(TO_PVOID(pAddress), &uValue, sizeof(uValue));
}

template <typename T>
void PatchStr(T pAddress, const char* sValue) {
    PatchMemory(TO_PVOID(pAddress), TO_PVOID(sValue), strlen(sValue));
}

// 不包括结尾的
template <typename T, typename U>
void PatchNop(T pAddress, U pDestination) {
    size_t uSize = TO_UINTPTR(pDestination) - TO_UINTPTR(pAddress);
    void* pValue = malloc(uSize);
    memset(pValue, 0x90, uSize);
    PatchMemory(TO_PVOID(pAddress), pValue, uSize);
    free(pValue);
}

template <typename T, typename U>
void PatchJmp(T pAddress, U pDestination) {
    Patch1(pAddress, 0xE9);
    Patch4(pAddress + 1, TO_UINTPTR(pDestination) - TO_UINTPTR(pAddress) - 5);
}



template <typename T, typename U>
void PatchCall(T pAddress, U pDestination, size_t uSize = 5) {
    if (uSize < 5) {
        ErrorMessage("Cannot PatchCall at 0x%08X with uSize = %d", TO_UINTPTR(pAddress), uSize);
        return;
    }
    Patch1(pAddress, 0xE8);
    Patch4(pAddress + 1, TO_UINTPTR(pDestination) - TO_UINTPTR(pAddress) - 5);
    if (uSize > 5) {
        PatchNop(pAddress + 5, pAddress + uSize);
    }
}

template <typename T, typename U>
void PatchJmp2(T pAddress, U pDestination, size_t uSize = 5) {
    if (uSize < 5) {
        ErrorMessage("Cannot PatchJmp at 0x%08X with uSize = %d", TO_UINTPTR(pAddress), uSize);
        return;
    }
    Patch1(pAddress, 0xE9); // 0xE9 是 JMP，绝对不会动堆栈！
    Patch4(pAddress + 1, TO_UINTPTR(pDestination) - TO_UINTPTR(pAddress) - 5);
    if (uSize > 5) {
        PatchNop(pAddress + 5, pAddress + uSize);
    }
}


template <typename T>
void PatchRetZero(T pAddress) {
    PatchStr(pAddress, "\x33\xC0\xC3");
}