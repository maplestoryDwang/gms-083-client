#include "pch.h"
#include "hook.h"

int get_screen_width();
void ApplyDynamicSlotPatches();


// ==========================================
// 动态调试配置中心 (可以在外面或运行时随时修改)
// ==========================================
int g_nCurrentSlotCount = 30;   // 当前使用的快捷键数量

const int g_nExpandLen = 30 * 12; // 拓展的长度

const int g_nMaxSlotCount = 30; // 数组开辟的最大空间上限，用于防止内存越界
const int g_nSSlotCount = 8; // 数组开辟的最大空间上限，用于防止内存越界
const int g_nMSlotCount = 12; // 数组开辟的最大空间上限，用于防止内存越界

// ==========================================
// 动态计算辅助函数 (自动生成 0x7C, 0x78 等参数)
// ==========================================
inline unsigned char GetSlotCountHex() {
    return static_cast<unsigned char>(g_nCurrentSlotCount); // 对应原 0x1E (30)
}

inline unsigned char GetArrayLengthHex() {
    return static_cast<unsigned char>(g_nCurrentSlotCount * 4); // 对应原 0x78 (120)
}

inline unsigned char GetOldArrayOffsetHex() {
    return static_cast<unsigned char>(4 + (g_nCurrentSlotCount * 4)); // 对应原 0x7C (124)
}



// 改为设置完第一排设置第二排
unsigned char Array_aDefaultQKM[] = {
    42, 0, 0, 0, // shift
    82, 0, 0, 0, // ins
    71, 0, 0, 0, // hm
    73, 0, 0, 0, // pup
    2, 0, 0, 0,  // 1
    3, 0, 0, 0,  // 2
    4, 0, 0, 0,  // 3
    5, 0, 0, 0,
    6, 0, 0, 0,  // 5
    30, 0, 0, 0, // a
    31, 0, 0, 0,
    32, 0, 0, 0,
    33, 0, 0, 0,
    29, 0, 0, 0,
    83, 0, 0, 0,
    79, 0, 0, 0,
    81, 0, 0, 0,
    16, 0, 0, 0,
    17, 0, 0, 0,
    18, 0, 0, 0,
    19, 0, 0, 0,
    20, 0, 0, 0,
    44, 0, 0, 0,
    45, 0, 0, 0,
    46, 0, 0, 0,
    47, 0, 0, 0,
    52, 0, 0, 0,
    7, 0, 0, 0,
    8, 0, 0, 0,
    9, 0, 0, 0,

};

// x ,y 都是 + 35
//unsigned char Array_ptShortKeyPos[] = {
//    // 第一排
//    7, 0, 0, 0, 8, 0, 0, 0,
//    42, 0, 0, 0, 8, 0, 0, 0,
//    77, 0, 0, 0, 8, 0, 0, 0,
//    112, 0, 0, 0, 8, 0, 0, 0,
//    147, 0, 0, 0, 8, 0, 0, 0,
//    182, 0, 0, 0, 8, 0, 0, 0,
//    217, 0, 0, 0, 8, 0, 0, 0,
//    252, 0, 0, 0, 8, 0, 0, 0,
//
//    31, 1, 0, 0, 8, 0, 0, 0,
//    66, 1, 0, 0, 8, 0, 0, 0,
//    101, 1, 0, 0, 8, 0, 0, 0,
//    136, 1, 0, 0, 8, 0, 0, 0,
//    171, 1, 0, 0, 8, 0, 0, 0,
//    206, 1, 0, 0, 8, 0, 0, 0,
//    241, 1, 0, 0, 8, 0, 0, 0,
//    20, 2, 0, 0, 8, 0, 0, 0,
//
//
//    // 第二排
//    7, 0, 0, 0, 41, 0, 0, 0,
//    42, 0, 0, 0, 41, 0, 0, 0,
//    77, 0, 0, 0, 41, 0, 0, 0,
//    112, 0, 0, 0, 41, 0, 0, 0,
//    147, 0, 0, 0, 41, 0, 0, 0,
//    182, 0, 0, 0, 41, 0, 0, 0,
//    217, 0, 0, 0, 41, 0, 0, 0,
//    252, 0, 0, 0, 41, 0, 0, 0,
//
//    31, 1, 0, 0, 41, 0, 0, 0,
//    66, 1, 0, 0, 41, 0, 0, 0,
//    101, 1, 0, 0, 41, 0, 0, 0,
//    136, 1, 0, 0, 41, 0, 0, 0,
//    171, 1, 0, 0, 41, 0, 0, 0,
//    206, 1, 0, 0, 41, 0, 0, 0,
//    241, 1, 0, 0, 41, 0, 0, 0,
//    20, 2, 0, 0, 41, 0, 0, 0
//};

// 直接定义成 int 数组，读写极其直观
int Array_ptShortKeyPos[] = {
    // 第一排 (X, Y)
    7, 8, 42, 8, 77, 8, 112, 8, 147, 8, 182, 8, 217, 8, 252, 8,
    287, 8, 322, 8, 357, 8, 392, 8, 427, 8, 462, 8, 497, 8, // 这里你想写多大就写多大！

    // 第二排 (X, Y)
    7, 41, 42, 41, 77, 41, 112, 41, 147, 41, 182, 41, 217, 41, 252, 41,
    287, 41, 322, 41, 357, 41, 392, 41, 427, 41, 462, 41, 497, 41
};


// old键盘 120
unsigned char Array_aDefaultQKM_0[g_nMaxSlotCount * 4]; // 动态初始化，大小与最大支持格子匹配

// 360字节的扩展数组 (30 * 12)
unsigned char Array_Expanded[g_nExpandLen];

// 120 = 30 x 4 不知道为什么多给4
//unsigned char cooldown_Array[124]; // 动态分配和初始化
unsigned char cooldown_Array[g_nMaxSlotCount * 4]; // 动态分配和初始化

DWORD Array_aDefaultQKM_Address = (DWORD)&Array_aDefaultQKM;
DWORD Array_Expanded_Address = (DWORD)&Array_Expanded;

// ==========================================
// 汇编中转裸函数 (注意内部的硬编码 0x1E 和 0x7C 也被外部变量动态化了)
// ==========================================
DWORD CompareValidate_Retn = 0x008DD8BD;
DWORD sub_9FA0CB_cave_retn_1 = 0x009FA0E1;

__declspec(naked) void CompareValidateFuncKeyMappedInfo_cave() {
    _asm {
        push 0x168   // 60h -> 168H    8键 - 》 30键  12X30= 360 = 0x168    32键盘 ->  384（0x180）
        push 0x0
        push eax
        pushad
        popad
        jmp CompareValidate_Retn
    }
}

__declspec(naked) void sub_9FA0CB_cave() {
    _asm {
        test eax, eax
        jne  label
        push 0xF4         // 0x44 -》0xF4       一个结构体大小44   0x4偏移 + 20hx（32 = 4X8） + 20h     0x4 + 30*8= 240(F0)    0x4 + 32*8 = 104
        pushad
        popad
        jmp  sub_9FA0CB_cave_retn_1
    label:
        push 0x168
        push 0x0
        push eax
        pushad
        popad
        jmp  CompareValidate_Retn
    }
}

__declspec(naked) void sDefaultQuickslotKeyMap_cave() {
    _asm {
        push ebx
        push esi
        push edi
        xor  edx, edx
        mov  ebx, ecx
        call label
        nop
        lea  edi, dword ptr ds:[ebx + 0x4]

        // 动态读取全局变量，不再硬编码 0x1E 和 0x7C
        mov  ecx, g_nCurrentSlotCount 
        mov  esi, Array_aDefaultQKM_Address
        rep  movsd

                        // 动态计算 Old 数组的目标位置
        mov  eax, g_nCurrentSlotCount
        shl  eax, 2 // eax = g_nCurrentSlotCount * 4
        add  eax, 4 // eax = 4 + 新数组长度
        lea  edi, dword ptr ds:[ebx + eax]
        
        mov  ecx, g_nCurrentSlotCount
        mov  esi, Array_aDefaultQKM_Address
        rep  movsd
        
        pop  edi
        pop  esi
        pop  ebx
        ret
    label:
        push esi
        mov  esi, ecx
        lea  eax, dword ptr ds:[esi + 0x4]
        push 0x0072B7C2
        ret
    }
}

__declspec(naked) void DefaultQuickslotKeyMap_cave() {
    _asm {
        push esi
        push edi
        lea  eax, dword ptr ds:[ecx + 0x4]
        mov  esi, Array_aDefaultQKM_Address
        mov  ecx, g_nCurrentSlotCount // 动态读取
        mov  edi, eax
        rep  movsd
        pop  edi
        pop  esi
        ret
    }
}

__declspec(naked) void Restore_Array_Expanded_cave() {
    _asm {
        push edi
        push ecx
        xor  eax, eax
        mov  edi, Array_Expanded_Address

                // 360 字节 / 4 = 90 (0x5A)
                // 保持清理整个缓冲区以确保安全
        mov  ecx, 0x5A 
        rep  stosd
        pop  ecx
        pop  edi
        lea  eax, [esi + 0xD7C]
        push 0x008CFE03
        ret
    }
}

// ==========================================
// 游戏引擎原厂指针/函数定义
// ==========================================
#define SP_QUICKSLOT_INDEX 2504
static const char** g_ppStringPool = reinterpret_cast<const char**>(0x00BDC9D4);

typedef void*(__cdecl* StringPool_GetInstance_t)();
static StringPool_GetInstance_t StringPool_GetInstance = reinterpret_cast<StringPool_GetInstance_t>(0x0079E805);

typedef int(__thiscall* Layer_GetDim_t)(void* pLayer);
static Layer_GetDim_t Layer_GetWidth = reinterpret_cast<Layer_GetDim_t>(0x00440C00);
static Layer_GetDim_t Layer_GetHeight = reinterpret_cast<Layer_GetDim_t>(0x00440C2A);

typedef int(__stdcall* Canvas_Clear_t)(void* pCanvas, int x, int y, int w, int h, int color);

static char* g_pQuickSlotS_encoded = nullptr;
static char* g_pQuickSlotM_encoded = nullptr;
static const char* g_pQuickSlot_stock = nullptr;
static int g_nLastQuickSlotProfile = -1;

// ==========================================
// 游戏内实时刷新与分辨率同步 (包含运行时动态补丁)
// ==========================================
void __cdecl RefreshQuickSlotArt() {
    int nWidth = get_screen_width();
    int nProfile = (nWidth == 800) ? 0 : (nWidth == 1024) ? 1
                                                          : 2;

    if (nProfile == g_nLastQuickSlotProfile) {
        return;
    }
    g_nLastQuickSlotProfile = nProfile;

    const char* pFrame;
    switch (nProfile) {
    case 0:
        pFrame = g_pQuickSlotS_encoded;
        g_nCurrentSlotCount = g_nSSlotCount; // 动态改变全局变量
        break;
    case 1:
        pFrame = g_pQuickSlotM_encoded;
        g_nCurrentSlotCount = g_nMSlotCount; // 动态改变全局变量
        break;
    default:
        pFrame = g_pQuickSlot_stock;
        g_nCurrentSlotCount = g_nMaxSlotCount; // 恢复最大上限
        break;
    }

    g_ppStringPool[SP_QUICKSLOT_INDEX] = pFrame;

    int* pInstance = static_cast<int*>(StringPool_GetInstance());
    if (pInstance && pInstance[0]) {
        reinterpret_cast<int*>(pInstance[0])[SP_QUICKSLOT_INDEX] = 0;
    }

    // 重新应用动态补丁，把计算出的新参数动态写进内存
    ApplyDynamicSlotPatches();
}

void __cdecl ClearQuickSlotFrame(void* pThis, void* pFrameCanvas) {
    if (!pThis || !pFrameCanvas)
        return;
    void* pLayer = *reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0xCC4);
    if (!pLayer)
        return;
    int w = Layer_GetWidth(pLayer);
    int h = Layer_GetHeight(pLayer);
    void** vtbl = *reinterpret_cast<void***>(pFrameCanvas);
    reinterpret_cast<Canvas_Clear_t>(vtbl[0x8C / 4])(pFrameCanvas, 0, 0, w, h, 0xFFFFFF);
}

DWORD QuickSlotArt_Retn = 0x008DDE79;
__declspec(naked) void QuickSlotArt_cave() {
    _asm {
        pushad
        call RefreshQuickSlotArt
        mov  eax, [ebp - 0x18]
        push eax
        mov  eax, [ebp - 0x3C]
        push eax
        call ClearQuickSlotFrame
        add  esp, 8
        popad
        push 0x9C8  
        jmp  QuickSlotArt_Retn
    }
}

// ==========================================
// 新增：核心动态补丁刷新器
// ==========================================
void ApplyDynamicSlotPatches() {
    unsigned char slots = GetSlotCountHex();       // 比如 0x1E
    unsigned char len = GetArrayLengthHex();       // 比如 0x78
    unsigned char offset = GetOldArrayOffsetHex(); // 比如 0x7C

    // 1. 动态改写循环格子数量 (原本的 0x1E 集合)
    Patch1(0x008DD916, slots);
    Patch1(0x008DD8AD, slots);
    Patch1(0x008DE941 + 2, slots);
    Patch1(0x008E099F + 3, slots);
    Patch1(0x004F928A + 2, slots);
    Patch1(0x004F93F9 + 2, slots);

    // 2. 动态改写 Draw 函数里的范围限制（原本的 0x7C）
    Patch1(0x008DE75E + 3, offset);

    // 3. 动态改写各类结构体 Old 数组位置偏移补丁（原 0x7C 与 0x78 的集合）
    Patch1(0x00833797 + 2, offset);
    Patch1(0x00833841 + 2, offset);
    Patch1(0x00833791 + 1, len);
    Patch1(0x0083383B + 1, len);

    Patch1(0x0083287F + 2, offset);
    Patch1(0x00832882 + 1, len);

    Patch1(0x0072B8C0 + 2, offset);
    Patch1(0x0072B8A0 + 1, len);
    Patch1(0x0072B8BD + 1, len);

    Patch1(0x0072B83E + 1, len);
    Patch1(0x0072B861 + 1, len);
    Patch1(0x0072B867 + 2, offset);

    Patch1(0x00836A1E + 1, len);
    Patch1(0x00836A21 + 2, offset);

    // 4. 动态改写坐标数组结尾边界判断
    Patch4(0x008DE926 + 2, (DWORD)&Array_ptShortKeyPos + 4 + g_nCurrentSlotCount * 8);
}

// ==========================================
// 初始化 Hook 入口
// ==========================================
void AttachQuickSlotsMod() {
    // 内存初始化
    memset(Array_aDefaultQKM_0, 0, sizeof(Array_aDefaultQKM_0));
    memcpy(Array_aDefaultQKM_0, Array_aDefaultQKM, sizeof(Array_aDefaultQKM));
    memset(cooldown_Array, 0xFF, sizeof(cooldown_Array));
    memset(Array_Expanded, 0, sizeof(Array_Expanded));

    // 重新绘制状态栏宽度
    // 03f0H  
    Patch1(0x008D155C + 1, 0xf0);
    Patch1(0x008D155C + 2, 0x03);

    Patch1(0x008D182E + 1, 0xf0);
    Patch1(0x008D182E + 2, 0x03);

    Patch1(0x008D1AC0 + 1, 0xf0);
    Patch1(0x008D1AC0 + 2, 0x03);

    // 设置默认与旧的键盘指针
    Patch4(0x0072B7CE + 1, (DWORD)&Array_aDefaultQKM_0);
    Patch4(0x0072B8EB + 1, (DWORD)&Array_aDefaultQKM_0);

    // 劫持指针指向扩展数组
    Patch1(0x008DD8FD, 0xBB); // mov ...
    Patch4(0x008DD8FD + 1, (DWORD)&Array_Expanded);
    Patch1(0x008DD8FD + 5, 0x90);

    Patch1(0x008DD898, 0xB8);
    Patch4(0x008DD898 + 1, (DWORD)&Array_Expanded);
    Patch1(0x008DD898 + 5, 0x90);

    Patch1(0x008DDF99, 0xB8);
    Patch4(0x008DDF99 + 1, (DWORD)&Array_Expanded);
    PatchNop(0x008DDF99 + 5, 0x008DDF99 + 8);

    // 快捷键索引重定向
    Patch1(0x008D7F1E + 1, 0x34);
    Patch1(0x008D7F1E + 2, 0x85);
    Patch4(0x008D7F1E + 3, (DWORD)&Array_Expanded);

    // 坐标分配
    Patch4(0x008DE94D + 2, (DWORD)&Array_ptShortKeyPos);
    Patch4(0x008DE955 + 2, (DWORD)&Array_ptShortKeyPos + 4);

    Patch4(0x008DE8F4 + 1, (DWORD)&Array_ptShortKeyPos + 4);

    // 冷却时间重定向
    Patch1(0x008E069D, 0xBE);
    Patch4(0x008E069D + 1, (DWORD)&cooldown_Array);
    Patch1(0x008E069D + 5, 0x90);
    Patch1(0x008E06A3, 0xBF);
    Patch4(0x008E06A3 + 1, (DWORD)&Array_Expanded + 1);
    Patch1(0x008E06A3 + 5, 0x90);

    // 执行首次动态计算补丁应用
    ApplyDynamicSlotPatches();

    // 加载资源皮肤并应用中转跳
    g_pQuickSlotS_encoded = EncodeStringAlloc("UI/StatusBar.img/base/quickSlotS");
    g_pQuickSlotM_encoded = EncodeStringAlloc("UI/StatusBar.img/base/quickSlotM");
    g_pQuickSlot_stock = g_ppStringPool[SP_QUICKSLOT_INDEX];

    PatchJmp(0x008DDE74, &QuickSlotArt_cave);
    PatchJmp(0x008DD8B8, &CompareValidateFuncKeyMappedInfo_cave);
    PatchJmp(0x009FA0DB, &sub_9FA0CB_cave);
    PatchJmp(0x0072B7BC, &sDefaultQuickslotKeyMap_cave);
    PatchJmp(0x0072B8E6, &DefaultQuickslotKeyMap_cave);
    PatchJmp(0x008CFDFD, &Restore_Array_Expanded_cave);
    PatchNop(0x008CFDFD + 5, 0x008CFDFD + 6);
}