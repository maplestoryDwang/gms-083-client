#include "pch.h"
#include "hook.h"
#include "resolution.h"

int get_screen_width();

// 15 X2 键  上1 下1 上2 下2排列
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
    23, 0, 0, 0,
    24, 0, 0, 0,

};

// x ,y 坐标 x+35 y+33
unsigned char Array_ptShortKeyPos[] = {
    7, 0, 0, 0, 8, 0, 0, 0,
    7, 0, 0, 0, 41, 0, 0, 0,
    42, 0, 0, 0, 8, 0, 0, 0,
    42, 0, 0, 0, 41, 0, 0, 0,
    77, 0, 0, 0, 8, 0, 0, 0,
    77, 0, 0, 0, 41, 0, 0, 0,
    112, 0, 0, 0, 8, 0, 0, 0,
    112, 0, 0, 0, 41, 0, 0, 0,
    147, 0, 0, 0, 8, 0, 0, 0,
    147, 0, 0, 0, 41, 0, 0, 0,
    182, 0, 0, 0, 8, 0, 0, 0,
    182, 0, 0, 0, 41, 0, 0, 0,
    217, 0, 0, 0, 8, 0, 0, 0,
    217, 0, 0, 0, 41, 0, 0, 0,
    252, 0, 0, 0, 8, 0, 0, 0,
    252, 0, 0, 0, 41, 0, 0, 0,
    31, 1, 0, 0, 8, 0, 0, 0,
    31, 1, 0, 0, 41, 0, 0, 0,
    66, 1, 0, 0, 8, 0, 0, 0,
    66, 1, 0, 0, 41, 0, 0, 0,
    101, 1, 0, 0, 8, 0, 0, 0,
    101, 1, 0, 0, 41, 0, 0, 0,
    136, 1, 0, 0, 8, 0, 0, 0,
    136, 1, 0, 0, 41, 0, 0, 0,
    171, 1, 0, 0, 8, 0, 0, 0,
    171, 1, 0, 0, 41, 0, 0, 0,
    206, 1, 0, 0, 8, 0, 0, 0,
    206, 1, 0, 0, 41, 0, 0, 0,
    241, 1, 0, 0, 8, 0, 0, 0,
    241, 1, 0, 0, 41, 0, 0, 0,
    20 , 2, 0, 0, 8, 0, 0, 0,
    20 , 2, 0, 0, 41, 0, 0, 0
};




// old键盘  120
unsigned char Array_aDefaultQKM_0[] = {
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
    23, 0, 0, 0,
    24, 0, 0, 0,
};


// 360 / 30 = 12
// 是否初始值有用？
// 32*12 = 384 0x180
unsigned char Array_Expanded[384] = {
    4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 53, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 124 / 4 = 31 why?
// 32*4 = 128
unsigned char cooldown_Array[132] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

DWORD Array_aDefaultQKM_Address = (DWORD)&Array_aDefaultQKM;
DWORD Array_Expanded_Address = (DWORD)&Array_Expanded;

DWORD CompareValidate_Retn = 0x008DD8BD;
DWORD sub_9FA0CB_cave_retn_1 = 0x009FA0E1;

__declspec(naked) void CompareValidateFuncKeyMappedInfo_cave() {
    _asm {
		push 0x180   // 360 是0x168   384 0x180 
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
		push 0x104    // 结构体大小    4+ F0（8X30键）   4+8X32 = 260 -> 0x104
		pushad
		popad
		jmp  sub_9FA0CB_cave_retn_1
	label:
		push 0x180
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
		mov  ecx, 0x20                    // 30-32  1E - 20
		mov  esi, Array_aDefaultQKM_Address
		rep  movsd
		lea  edi, dword ptr ds:[ebx + 0x84]     //7C/124 = 30X4+4   -> 32x4+4 = 132 0x84
		mov  ecx, 0x20
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
		mov  ecx, 0x20
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
        // 384 /4 =96 0x60
		mov  ecx, 0x60
		rep  stosd
		pop  ecx
		pop  edi
		lea  eax, [esi + 0xD7C]
		push 0x008CFE03
		ret
    }
}


// 008DE7DA CUIStatusBar::HitTest(CUIStatusBar *this, signed int a2, signed int a3, struct CCtrlWnd **a4)


/*
* 32已经不够了需要hook
___:008DE75E 83 7D B8 24                       cmp     [ebp+var_48], 24h ; '$' ; Compare Two Operands
___:008DE762 0F 8C 46 F8 FF FF                 jl      loc_8DDFAE      ; Jump if Less (SF!=OF)
*/
// 劫持起点是 0x008DE75E，连续吞掉 10 字节后，下一条安全的返回地址是 0x008DE768
DWORD DrawLimit_Retn_Normal = 0x008DE768; // 正常向下执行的返回点
DWORD DrawLimit_Retn_Jump = 0x008DDFAE;   // 如果小于(jl)需要跳转的目标地址

__declspec(naked) void DrawLimit_cave() {
    _asm {
        // 直接对内存地址与 132 (0x84) 进行 32 位大整数比较
        // 这样既安全、效率高，又 100% 不会污染任何寄存器！
        cmp dword ptr [ebp - 0x48], 132

        // 恢复原本的 jl 逻辑
        jl loc_less

                // 大于等于 132，循环结束，正常往下走
        jmp DrawLimit_Retn_Normal

    loc_less:
        // 小于 132，继续循环，跳回循环体内
        jmp DrawLimit_Retn_Jump
    }
}



#define SP_QUICKSLOT_INDEX 2504

static const char** g_ppStringPool = reinterpret_cast<const char**>(0x00BDC9D4);

typedef void*(__cdecl* StringPool_GetInstance_t)();
static StringPool_GetInstance_t StringPool_GetInstance = reinterpret_cast<StringPool_GetInstance_t>(0x0079E805);

typedef int(__thiscall* Layer_GetDim_t)(void* pLayer);
static Layer_GetDim_t Layer_GetWidth = reinterpret_cast<Layer_GetDim_t>(0x00440C00);
static Layer_GetDim_t Layer_GetHeight = reinterpret_cast<Layer_GetDim_t>(0x00440C2A);

typedef int(__stdcall* Canvas_Clear_t)(void* pCanvas, int x, int y, int w, int h, int color);

// 图片指针
static char* g_pQuickSlotS_encoded = nullptr;
static char* g_pQuickSlotM_encoded = nullptr;
static const char* g_pQuickSlot_stock = nullptr;
static int g_nLastQuickSlotProfile = -1;

void __cdecl RefreshQuickSlotArt() {
    int nWidth = get_screen_width();
    int nProfile = (nWidth == 800) ? 0 : (nWidth == 1024) ? 1
                                                          : 2;
    if (nProfile == g_nLastQuickSlotProfile) {
        return;
    }
    g_nLastQuickSlotProfile = nProfile;

    const char* pFrame;
    int nSlots;
    switch (nProfile) {
    case 0:
        pFrame = g_pQuickSlotS_encoded;
        nSlots = 8;
        break;
    case 1:
        pFrame = g_pQuickSlotM_encoded;
        nSlots = 12;
        break;
    default:
        pFrame = g_pQuickSlot_stock;
        nSlots = 32;
        break;
    }

    g_ppStringPool[SP_QUICKSLOT_INDEX] = pFrame;

    int* pInstance = static_cast<int*>(StringPool_GetInstance());
    if (pInstance && pInstance[0]) {
        reinterpret_cast<int*>(pInstance[0])[SP_QUICKSLOT_INDEX] = 0;
    }

    // sub_8E0634
    // // VARIANTARG *__thiscall CUIStatusBar::CQuickSlot::Draw(char *this)
    //Patch1(0x008DE75E + 3, static_cast<unsigned char>(4 + nSlots * 4));  // 0x24 = 4 + 0x20 =4+ 8*4   124 = 7C     // 这里32键位有问题，导致无法渲染。32 X4 + 4 > 128
    // 从 0x008DE75E 开始劫持 10 字节
    PatchJmp(0x008DE75E, &DrawLimit_cave);

    // 抹平残余的 5 字节
    Patch1(0x008DE75E + 5, 0x90);
    Patch1(0x008DE75E + 6, 0x90);
    Patch1(0x008DE75E + 7, 0x90);
    Patch1(0x008DE75E + 8, 0x90);
    Patch1(0x008DE75E + 9, 0x90);


    Patch1(0x008E099F + 3, static_cast<unsigned char>(nSlots));
}




void __cdecl ClearQuickSlotFrame(void* pThis, void* pFrameCanvas) {
    if (!pThis || !pFrameCanvas) {
        return;
    }
    void* pLayer = *reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0xCC4);
    if (!pLayer) {
        return;
    }
    void** vtbl = *reinterpret_cast<void***>(pFrameCanvas);
    reinterpret_cast<Canvas_Clear_t>(vtbl[0x8C / 4])(pFrameCanvas, 0, 0, w, h, 0xFFFFFF); // 虚函数
}

DWORD QuickSlotArt_Retn = 0x008DDE79;

__declspec(naked) void QuickSlotArt_cave() {
    _asm {
		pushad
		call RefreshQuickSlotArt
		mov  eax, [ebp - 0x18]
		push eax
		mov  eax, [ebp - 0x3C]               // 60
		push eax
		call ClearQuickSlotFrame
		add  esp, 8                         //堆栈平衡
		popad
		push 0x9C8 // 补回游戏原本在 0x008DDE74 左右被你擦除掉的原始指令
		jmp  QuickSlotArt_Retn
    }
}



DWORD KeyConfig_Retn = 0x0083379A; // 0x00833797 + 3 字节 lea 的下一条指令地址

__declspec(naked) void KeyConfig_OldArray_cave() {
    _asm {
        // 1. 补回并修正 push 20h -> 改为标准的 4 字节 push 0x80，绝不溢出
        push 0x80 // 机器码是 68 80 00 00 00

        // 2. 补回 lea eax, [edi+4] 和 push eax
        lea  eax, dword ptr ds:[edi + 4]
        push eax

                           // 3. 完美替代 lea eax, [edi+84h] 的逻辑
                           // 此时 eax 已经是 edi+4 了，直接加上 128 (0x80) 就是我们要的 edi+132 (0x84)！
        add  eax, 0x80

        // 4. 跳回原程序继续执行
        jmp  KeyConfig_Retn
    }
}


DWORD KeyConfig_Destroy_Retn = 0x00833845; // 返回到 call _memcmp

__declspec(naked) void KeyConfig_Destroy_cave() {
    _asm {
        push 0x80 // 标准 4 字节压栈，值为 128 (0x80)
        lea  eax, [edi+4]
        push eax
        add  eax, 0x80 // 此时 eax 是 edi+4，加 128 刚好等于 edi+132 (0x84)
        push eax
        jmp  KeyConfig_Destroy_Retn
    }
}


DWORD sub_832834_Retn = 0x00832884; // 正常返回下一条指令

__declspec(naked) void sub_832834_cave() {
    _asm {
        mov  eax, dword ptr ds:[0x00BED658] // 补回 mov eax
        lea  ecx, [eax + 0x84] // 升级为完整 32 位 lea 偏移 (132)
        push 0x80 // 升级为完整 4 字节 push (128)
        jmp  sub_832834_Retn
    }
}


DWORD SaveQKM_Part1_Retn = 0x0072B8A6;

__declspec(naked) void SaveQKM_Part1_cave() {
    _asm {
        push 0x80 // 发送 128 字节长度
        lea  edi, [esi+4]
        push edi
        jmp  SaveQKM_Part1_Retn
    }
}
DWORD SaveQKM_Part2_Retn = 0x0072B8C4;

__declspec(naked) void SaveQKM_Part2_cave() {
    _asm {
        push 0x80 // 1. 压入长度 128
        push edi // 2. 原来的 push edi
        add  esi, 0x84 // 3. 升级为 32 位大整数加法，加 132 字节 (0x84)
        push esi // 4. 原来的 push esi
        jmp  SaveQKM_Part2_Retn
    }
}

DWORD OnInit_Part1_Retn = 0x0072B844;

__declspec(naked) void OnInit_Part1_cave() {
    _asm {
        push 0x80 // 解包 128 字节
        lea  eax, [esi+4]
        push eax
        jmp  OnInit_Part1_Retn
    }
}

DWORD OnInit_Part2_Retn = 0x0072B86B;

__declspec(naked) void OnInit_Part2_cave() {
    _asm {
        push 0x80 // 新数组长 128
        lea  eax, [esi+4]
        push eax
        add  eax, 0x80 // 此时 eax = esi+4，加上 128 就是 Old 数组的偏移 132 (0x84)
        push eax
        jmp  OnInit_Part2_Retn
    }
}


DWORD sub_8369D0_Retn = 0x00836A25;

__declspec(naked) void sub_8369D0_cave() {
    _asm {
        push 0x80 // 比较长度改为 128
        push ecx
        add  eax, 0x84 // 升级为 32 位加法，加 132 字节 (0x84)
        push eax
        jmp  sub_8369D0_Retn
    }
}


DWORD StatusBarClickRange_Rtn_Hook = 0x008DE850; // hook开始的地方
DWORD StatusBarClickRange_Rtn_INT = 0x008DE8CC;  // 可以点击
DWORD StatusBarClickRange_Rtn_ZERO = 0x008DE8C8; // 不能点击
DWORD StatusBarClickRange_Rtn; // 返回值


int m_nChatWndType;   // 聊天窗状态 1=隐藏 3=显示
int m_nChatWndHeight; // 聊天窗高度 最大值为489
int rx;               // 鼠标位置，从statusbar算起，而非整个游戏窗口
int ry;               // 鼠标位置，从statusbar算起，而非整个游戏窗口
int m_bDragChatWnd;   // 是否拖拽状态
int quickSlotIsEnabled;
// 8DE8C8 - 8DE850 = 120 / 0x78
// 加上__declspec(naked) 保留ebp
// 只要你在 __declspec(naked) 函数的大括号里面写了诸如 int x; 这样的代码，这个函数就不再是真正的 “naked（裸）” 函数了，编译器会自动给它“穿上衣服”（生成 push ebp 函数头），从而把原程序的 ebp 寄存器踩死。用全局变量中转是逆向写外挂/补丁最标准的做法。

//__declspec(naked) void StatusBarClickRangeFunc() {
//
//
//    // 获取当时内存和寄存器中的参数
//    _asm {
//        push eax
//        // this->m_nChatWndType != 1 
//        mov  eax, [esi + 0D00h]               // m_nChatWndType = 3
//        mov  m_nChatWndType, eax
//
//                //  m_nChatWndHeight = this->m_nChatWndHeight;
//        mov  eax, [esi + 0CFCh]
//        mov  m_nChatWndHeight, eax                                // 46 = 70
//
//        // .text:0086D582 81 FF 41 02 00 00                       cmp     edi, 241h
//        // rx >= 577
//
//        mov  eax, [ebp+08h]
//        mov  rx, eax
//        mov  eax, [ebp+0Ch]
//        mov  ry, eax
//
//        //  this->m_bDragChatWnd
//        mov  eax, [esi + 0BB4h]
//        mov  m_bDragChatWnd, eax
//
//        // this->m_QuickSlot
//        mov  eax, [esi + 0D10h]        
//        mov  quickSlotIsEnabled, eax // 获取是否可以
//        pop  eax
//    }
//
//    int chatTopY = 507 - m_nChatWndHeight;
//
//    StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT; // 不可点击
//
//
//    int curWidth = w;
//
//
//
//
//    //if (curWidth > 800) {
//    //    if (m_nChatWndType == 1) {     // 原来x 489 y 506
//    //        if (rx < 1585 && ry > 506) // 鼠标在状态栏    1321 + 33*3 = 1420/16  | 1486是18键 | 1486 + 33*3=1585（21键时）
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //    } else {
//    //        if (rx < 564 && ry > chatTopY) // 鼠标在聊天框头部以下的区域
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //        if (rx < 1585 && ry > 506) // 鼠标在状态栏
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //        // if (m_bDragChatWnd == 1 && rx < 564 && ry > chatTopY) // 拖拽聊天框状态下鼠标在状态栏
//    //        //{
//    //        //     StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        // }
//    //    }
//    //} else {
//    //    if (m_nChatWndType == 1) {
//    //        if (rx < 1321 && ry > 506) // 鼠标在状态栏
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //        if (quickSlotIsEnabled == 1 && rx > 368 && ry > 413) // 鼠标在快捷栏
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //    } else {
//    //        if (rx < 564 && ry > chatTopY) // 鼠标在聊天框头部以下的区域
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //        if (rx < 1321 && ry > 506) // 鼠标在状态栏
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //        if (quickSlotIsEnabled == 1 && rx > 368 && ry > 413) // 鼠标在快捷栏
//    //        {
//    //            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
//    //        }
//    //    }
//    //}
//}

// ==================== 2. 纯 C++ 业务逻辑函数 ====================
void StatusBarClickRange_CPP() {
    int chatTopY = 507 - m_nChatWndHeight;
    int curWidth = get_screen_width(); 
    //StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT;
    int v7 = 507 - m_nChatWndHeight;
    int v8 = 507 - m_nChatWndHeight;


    // 区域 1：正常聊天窗判定区
    // 条件：rx < 580 并且 聊天窗处于显示状态 (m_nChatWndType == 1) 并且 ry < 507
    if (rx < 580 && m_nChatWndType == 1 && ry < 507) {
        StatusBarClickRange_Rtn =  StatusBarClickRange_Rtn_ZERO; // 不能点击
    }

    // 区域 2：左侧边界判定区（非拖拽状态）
    // 条件：rx < 80 并且 ry < v7 并且 没在拖拽聊天窗 (!m_bDragChatWnd)
    if (rx < 80 && ry < v7 && !m_bDragChatWnd) {
        StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_ZERO; // 不能点击
    }

    // 区域 3：中间非拖拽判定区
    // 条件：rx 在 80 ~ 580 之间 并且 ry < v8 并且 没在拖拽聊天窗 (!m_bDragChatWnd)
    if (rx >= 80 && rx <= 580 && ry < v8 && !m_bDragChatWnd) {
        StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_ZERO; // 不能点击
    }

    // 区域 4：右侧扩展判定区
    if (rx > 647) {
        // 情况 A：如果 *(this + 836) [即 quickSlotIsEnabled] 为真，但 ry < 427 时
        if (quickSlotIsEnabled && ry < 427) {
            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_ZERO; // 不能点击
        }
        // 情况 B：如果 *(this + 836) 为假，只要 ry < 507 时
        if (!quickSlotIsEnabled && ry < 507) {
            StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_ZERO; // 不能点击
        }
    }

    // 区域 5：右侧特定夹缝区
    // 条件：rx 在 580 ~ 647 之间 并且 ry < 507
    if (rx >= 580 && rx <= 647 && ry < 507) {
        StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_ZERO; // 不能点击
    }

    // 如果以上所有拦截区域都没命中的话，默认就是可以点击
    StatusBarClickRange_Rtn = StatusBarClickRange_Rtn_INT; 
}


// ==================== 3. 纯裸函数 Hook 入口 ====================
// 编译器绝对不会在这个函数里生成任何破坏 ebp 的垃圾指令！
__declspec(naked) void StatusBarClickRangeFunc() {
    __asm {
        // 1. 保护现场
        pushad

                // 2. 抓取原程序中纯正、安全的寄存器和堆栈参数，存入全局变量
        mov  eax, [esi + 0x0D00]               
        mov  m_nChatWndType, eax

        mov  eax, [esi + 0x0CFC]
        mov  m_nChatWndHeight, eax                         

        mov  eax, [ebp + 0x08] // 真正的原程序 rx 
        mov  rx, eax
        mov  eax, [ebp + 0x0C] // 真正的原程序 ry
        mov  ry, eax

        mov  eax, [esi + 0x0BB4]
        mov  m_bDragChatWnd, eax  // 0

        mov  eax, [esi + 0x0D10]        
        mov  quickSlotIsEnabled, eax

                // 3. 核心：调用 C++ 函数处理你后面复杂的 if-else 逻辑
                // 此时全局变量已被填满，C++ 函数可以直接用
        call StatusBarClickRange_CPP

                        // 4. 恢复现场
        popad
    }
}


// StatusBarClickRangeFunc 这个方法很重要，它会影响你鼠标的可点击区域。因为官方的客户端逻辑很乱。
void _declspec(naked) StatusBarClickRange() {
    _asm {
        call StatusBarClickRangeFunc
        jmp StatusBarClickRange_Rtn
    }
}



/*
* 根据结构体
00000000 CQuickslotKeyMappedMan struc ; (sizeof=0x44, align=0x4, copyof_7404)
00000000 __vftable       dd ?                    ; offset
00000004 m_aQuickslotKeyMapped dd 8 dup(?)
00000024 m_aQuickslotKeyMapped_Old dd 8 dup(?)
00000044 CQuickslotKeyMappedMan ends

旧的  0x04 + 0x20 = 0x24 是m_aQuickslotKeyMapped_Old的起始地址
起始地址是 0x04 + 新数组大小 0x78 = 0x7C

*/
void AttachQuickSlotsMod() {

    // CUIStatusBar::OnCreate(_DWORD *a1@<ecx>, int ebp0@<ebp>, int a3)
    // Layer = IWzGr2D::CreateLayer(dword_BF14EC, (ebp0 - 16), 0, 0,  97h, 80, 0, (ebp0 - 80), (ebp0 - 40));
    // 008D155C 68 97 00 00 00                    push    97h       // 97h -> 03F0h  1008
    Patch1(0x008D155C + 1, 0x36);                // 1008+35 = 1043  0x0413
    Patch1(0x008D155C + 2, 0x04);

    Patch1(0x008D182E + 1, 0x36);
    Patch1(0x008D182E + 2, 0x04);

    Patch1(0x008D1AC0 + 1, 0x36);
    Patch1(0x008D1AC0 + 2, 0x04);


    // hook dword_BDAFAC 2Ah, 52h, 47h, 49h, 1Dh, 53h, 4Fh, 51h
    // 设置旧的键盘
    // CQuickslotKeyMappedMan::CQuickslotKeyMappedMan
    Patch4(0x0072B7CE + 1, (DWORD)&Array_aDefaultQKM_0);
    // CQuickslotKeyMappedMan::DefaultQuickslotKeyMap(CQuickslotKeyMappedMan *this)
    Patch4(0x0072B8EB + 1, (DWORD)&Array_aDefaultQKM_0);


    // 8 -》30就是0x1E
    // CUIStatusBar::CQuickSlot::CompareValidateFuncKeyMappedInfo
    Patch1(0x008DD916, 0x20);   //1E - 20
    Patch1(0x008DD8AD, 0x20);   //1E - 20


    // CUIStatusBar::CQuickSlot::CompareValidateFuncKeyMappedInfo
    //  :008DD8FD 8D 9E 20 0D 00 00                 lea     ebx, [esi+0D20h] ; Load Effective Address
    Patch1(0x008DD8FD, 0xBB);                       // 0xBB 是 mov ebx, <32位立即数>
    Patch4(0x008DD8FD + 1, (DWORD)&Array_Expanded); // 填入新数组的内存地址
    Patch1(0x008DD8FD + 5, 0x90);                   // 0x90 是 NOP（空指令），用来对齐和填充旧指令剩下的垃圾字节

    // :008DD898 8D 86 20 0D 00 00                 lea     eax, [esi+0D20h] ; Load Effective Address
    Patch1(0x008DD898, 0xB8); // mov eax
    Patch4(0x008DD898 + 1, (DWORD)&Array_Expanded);
    Patch1(0x008DD898 + 5, 0x90);


    // CUIStatusBar::CQuickSlot::Draw
    // :008DE75E 83 7D B8 24                       cmp     [ebp+var_48], 24h ; '$' ; Compare Two Operands
    //Patch1(0x008DE75E + 3, 0x7C); //               124字节 和cooldown_Array相同 为什么又写一次？ 需要修改
    PatchJmp(0x008DE75E, &DrawLimit_cave);

    // 抹平残余的 5 字节
    Patch1(0x008DE75E + 5, 0x90);
    Patch1(0x008DE75E + 6, 0x90);
    Patch1(0x008DE75E + 7, 0x90);
    Patch1(0x008DE75E + 8, 0x90);
    Patch1(0x008DE75E + 9, 0x90);



    // ----------------------------------------------------------------------
    // CUIStatusBar::HitTest
    // ----------------------------------------------------------------------
    //Patch4(0x008DE850 + 1, 0x47F); // 可按区域580/   X + 571 = 
    //Patch4(0x008DE82B + 1, 368); // 可按区域  Y
    // 
    //  好像不需要resolution有加    ATTACH_HOOK(CUIStatusBar::GetShortCutIndexByPos, CUIStatusBar::GetShortCutIndexByPos_hook)
    // ;
    // todo 
    //PatchCall(StatusBarClickRange_Rtn_Hook, reinterpret_cast<uintptr_t>(&StatusBarClickRange), 120); // CUIStatusBar::HitTest 重构了





    /*
    ___:008DDF99 8B 45 C4                          mov     eax, [ebp+var_3C]
___    :008DDF9C 05 20 0D 00 00                    add     eax, 0D20h      ; Add
     */
    Patch1(0x008DDF99, 0xB8);
    Patch4(0x008DDF99 + 1, (DWORD)&Array_Expanded);
    PatchNop(0x008DDF99 + 5, 0x008DDF99 + 8);  // NOP 3


    // ___:008D7F1E 8D B4 83 1C 0D 00                 lea     esi, [ebx+eax*4+0D1Ch] ; Load Effective Address
    // 原版逻辑：eax 是快捷键的索引（0~7）。游戏通过 [ebx + eax*4 + 0xD1C]，先找到对象基址 ebx，加上结构体内部偏移 0xD1C，再乘以索引，算出了旧快捷键的内存地址。
    // 补丁逻辑：作者把机器码强行改成了 34 85。在 x86 汇编中，8D 34 85 刚好代表 lea esi, [eax * 4 + 立即数]。这样就直接扔掉了原本的结构体基址 ebx 和内部偏移，
    // 让 esi 直接指向了你新开辟的全局数组 Array_Expanded 对应的索引位置！
    Patch1(0x008D7F1E + 1, 0x34);
    Patch1(0x008D7F1E + 2, 0x85);
    Patch4(0x008D7F1E + 3, (DWORD)&Array_Expanded);  

    // 008DE94D 8B 88 B0 2D BE 00                 mov     ecx, dword_BE2DB0[eax]
    Patch4(0x008DE94D + 2, (DWORD)&Array_ptShortKeyPos);     // x
    // 008DE955 8B 80 B4 2D BE 00                 mov     eax, dword_BE2DB4[eax]
    Patch4(0x008DE955 + 2, (DWORD)&Array_ptShortKeyPos + 4); // y
    // 控制循环次数
    Patch1(0x008DE941 + 2, 0x20); // 设置值1E

    // signed int __stdcall CUIStatusBar::GetShortCutIndexByPos(int a1, int a2)
    // 设置esi进行比较
    // _:008DE8F4 BE B4 2D BE 00                    mov     esi, offset dword_BE2DB4
    Patch4(0x008DE8F4 + 1, (DWORD)&Array_ptShortKeyPos + 4); // y
    //   008DE926 81 FE F4 2D BE 00                 cmp     esi, offset dword_BE2DF4 ; Compare Two Operands
    Patch4(0x008DE926 + 2, (DWORD)&Array_ptShortKeyPos + 4 + 30 * 8); // 结尾  // 默认300 (12Ch)


    // sub_8E0634
    // 008E099F 83 7D E4 08                       cmp     [ebp+var_1C], 8 ; Compare Two Operands
    Patch1(0x008E099F + 3, 0x20);   // 1E - 20
    // 008E069D 8D B3 C4 0D 00 00                 lea     esi, [ebx+0DC4h] ; Load Effective Address
    Patch1(0x008E069D, 0xBE); // 0xBE：代表 mov esi, <32位立即数>
    Patch4(0x008E069D + 1, (DWORD)&cooldown_Array);
    Patch1(0x008E069D + 5, 0x90);

    // 008E06A3 8D BB 21 0D 00 00                 lea     edi, [ebx+0D21h] ; Load Effective Address
    Patch1(0x008E06A3, 0xBF);                             // 0xBF：代表 mov edi, <32位立即数>
    Patch4(0x008E06A3 + 1, (DWORD)&Array_Expanded + 1);
    Patch1(0x008E06A3 + 5, 0x90);                         // nop



    // int __thiscall CDraggableMenu::OnDropped(
    // 改变数值1E 30 / 7C 124 / 78 120
    // 004F928A 83 F8 08                          cmp     eax, 8          ; Compare Two Operands
    Patch1(0x004F928A + 2, 0x20);
    // 004F93F9 83 F8 08                          cmp     eax, 8          ; Compare Two Operands
    Patch1(0x004F93F9 + 2, 0x20);


    // CUIKeyConfig::OnDestroy

    // lea eax, [edi + 24h]，实际上就是让 eax 指向旧快捷键数组（Old）的起始位置。
    // .text:00833797  8D 47 24      lea eax, [edi + 24h]
    // .text:00833797  8D 47 7C      lea eax, [edi + 7Ch]

    /*
    ___:00833791 6A 20                             push    20h ; ' '       ; size_t                // 这里的 6A 代表 push byte（压入单字节立即数）。
    ___:00833793 8D 47 04                          lea     eax, [edi+4]    ; Load Effective Address
    ___:00833796 50                                push    eax             ; void *
    ___:00833797 8D 47 24                          lea     eax, [edi+24h]  ; Load Effective Address   // 这个有问题，不能直接eax, edi + 0x84
    */
    // 00833791 6A 20                             push    20h ; ' '       ; size_t

    // 有问题已改写
    //Patch1(0x00833791 + 1, 0x80);    // 新数组的长度  120 -》 128 0x80   // 又是有问题的偏移
    //Patch1(0x00833797 + 2, 0x84);    // 旧的数组偏移量  124 -> 132 0x84
    // 
    // 从 0x00833791 开始劫持，这 4 条指令总长 9 字节
    PatchJmp(0x00833791, &KeyConfig_OldArray_cave);

    // 用 NOP 填平剩下的 4 字节垃圾机器码 (9 - 5 = 4)
    Patch1(0x00833791 + 5, 0x90);
    Patch1(0x00833791 + 6, 0x90);
    Patch1(0x00833791 + 7, 0x90);
    Patch1(0x00833791 + 8, 0x90);






    /*
    ___:0083383B                   loc_83383B:                             ; CODE XREF: CUIKeyConfig::OnDestroy(void)+E9↑j
    ___:0083383B 6A 20                             push    20h ; ' '       ; size_t
    ___:0083383D 8D 47 04                          lea     eax, [edi+4]    ; Load Effective Address
    ___:00833840 50                                push    eax             ; void *
    ___:00833841 8D 47 24                          lea     eax, [edi+24h]  ; Load Effective Address
    ___:00833844 50                                push    eax             ; void *
    ___:00833845 E8 46 E0 22 00                    call    _memcmp         ; Call Procedure
    */
    //Patch1(0x0083383B + 1, 0x78);      // 0x78 -》 0x80
    //Patch1(0x00833841 + 2, 0x7C);      //  0x7C -> 0x84
    //  写入 Hook
    PatchJmp(0x0083383B, &KeyConfig_Destroy_cave);
    PatchNop(0x0083383B + 5, 0x0083383B + 9); // NOP 抹平余下的 4 字节




    // int __thiscall sub_832834(_DWORD *this)
    /*
        ___:0083287A A1 58 D6 BE 00                    mov     eax, dword_BED658
        ___:0083287F 8D 48 24                          lea     ecx, [eax+24h]  ; Load Effective Address
        ___:00832882 6A 20                             push    20h ; ' '       ; size_t
    */
    //Patch1(0x0083287F + 2, 0x7C);
    //Patch1(0x00832882 + 1, 0x78);

    // 写入 Hook
    PatchJmp(0x0083287A, &sub_832834_cave);
    PatchNop(0x0083287A + 5, 0x0083287A + 10); // NOP 抹平余下的 5 字节




    // int __thiscall CQuickslotKeyMappedMan::SaveQuickslotKeyMap(CQuickslotKeyMappedMan * this)
    /*
    * ___:0072B89C 83 65 FC 00                       and     [ebp+var_4], 0  ; Logical AND
        ___:0072B8A0 6A 20                             push    20h ; ' '       ; size_t
        ___:0072B8A2 8D 7E 04                          lea     edi, [esi+4]    ; Load Effective Address
        ___:0072B8A5 57                                push    edi             ; void *
        ___:0072B8A6 8D 4D E4                          lea     ecx, [ebp+var_1C] ; Load Effective Address
        ___:0072B8A9 E8 5E 07 D4 FF                    call    ?EncodeBuffer@COutPacket@@QAEXPBXI@Z ; Call Procedure
        ___:0072B8A9
        ___:0072B8AE 8B 0D 14 79 BE 00                 mov     ecx, dword_BE7914
        ___:0072B8B4 8D 45 E4                          lea     eax, [ebp+var_1C] ; Load Effective Address
        ___:0072B8B7 50                                push    eax
        ___:0072B8B8 E8 BE AA D6 FF                    call    ?SendPacket@CClientSocket@@QAEXABVCOutPacket@@@Z ; Call Procedure
        ___:0072B8B8
        ___:0072B8BD 6A 20                             push    20h ; ' '       ; size_t
        ___:0072B8BF 57                                push    edi             ; void *
        ___:0072B8C0 83 C6 24                          add     esi, 24h ; '$'  ; Add
        ___:0072B8C3 56                                push    esi             ; void *
        ___:0072B8C4 E8 37 53 33 00                    call    _memcpy         ; Call Procedure
    * 
    */
    // 0072B8A0 6A 20                             push    20h ; ' '       ; size_t
    //Patch1(0x0072B8A0 + 1, 0x78);
    // 写入 Hook
    PatchJmp(0x0072B8A0, &SaveQKM_Part1_cave);
    Patch1(0x0072B8A0 + 5, 0x90); // 抹平剩下的 1 字节


    //Patch1(0x0072B8BD + 1, 0x78);
    //Patch1(0x0072B8C0 + 2, 0x7C);
    // 写入 Hook
    PatchJmp(0x0072B8BD, &SaveQKM_Part2_cave);
    PatchNop(0x0072B8BD + 5, 0x0072B8BD + 7); // NOP 抹平剩下的 2 字节


    // int __thiscall CQuickslotKeyMappedMan::OnInit(char *this, int a2)

    /*
    ___:0072B83A 8B 4C 24 08                       mov     ecx, [esp+4+arg_0]
    ___:0072B83E 6A 20                             push    20h ; ' '       ; size_t
    ___:0072B840 8D 46 04                          lea     eax, [esi+4]    ; Load Effective Address
    ___:0072B843 50                                push    eax             ; void *
    ___:0072B844 E8 0E 6A D0 FF                    call    ?DecodeBuffer@CInPacket@@QAEXPAXI@Z ; Call Procedure
    */
    //Patch1(0x0072B83E + 1, 0x78);
    // 写入 Hook
    PatchJmp(0x0072B83E, &OnInit_Part1_cave);
    Patch1(0x0072B83E + 5, 0x90);



    /*
    ___:0072B861 6A 20                             push    20h ; ' '       ; size_t
    ___:0072B863 8D 46 04                          lea     eax, [esi+4]    ; Load Effective Address
    ___:0072B866 50                                push    eax             ; void *
    ___:0072B867 8D 46 24                          lea     eax, [esi+24h]  ; Load Effective Address
    ___:0072B86A 50                                push    eax             ; void *
    */
    //Patch1(0x0072B861 + 1, 0x78);
    //Patch1(0x0072B867 + 2, 0x7C);
    PatchJmp(0x0072B861, &OnInit_Part2_cave);
    PatchNop(0x0072B861 + 5, 0x0072B861 + 10);






    // __thiscall sub_8369D0
    /*
    ___:00836A1E 6A 20                             push    20h ; ' '       ; size_t
    ___:00836A20 51                                push    ecx             ; void *
    ___:00836A21 83 C0 24                          add     eax, 24h ; '$'  ; Add
    ___:00836A24 50                                push    eax             ; void *
    ___:00836A25 E8 66 AE 22 00                    call    _memcmp         ; Call Procedure
    */
    //Patch1(0x00836A1E + 1, 0x78);
    //Patch1(0x00836A21 + 2, 0x7C);

    // 写入 Hook
    PatchJmp(0x00836A1E, &sub_8369D0_cave);
    PatchNop(0x00836A1E + 5, 0x00836A1E + 7); // NOP 抹平剩下的 2 字节



    g_pQuickSlotS_encoded = EncodeStringAlloc("UI/StatusBar.img/base/quickSlotS");
    g_pQuickSlotM_encoded = EncodeStringAlloc("UI/StatusBar.img/base/quickSlotM");
    g_pQuickSlot_stock = g_ppStringPool[SP_QUICKSLOT_INDEX];

    // ___:008DDE74 68 C8 09 00 00                    push    9C8h            ; a3
    PatchJmp(0x008DDE74, &QuickSlotArt_cave);
    PatchJmp(0x008DD8B8, &CompareValidateFuncKeyMappedInfo_cave);
    PatchJmp(0x009FA0DB, &sub_9FA0CB_cave);

    // _DWORD *__fastcall CQuickslotKeyMappedMan::CQuickslotKeyMappedMan(_DWORD *a1)
    PatchJmp(0x0072B7BC, &sDefaultQuickslotKeyMap_cave);

    // CQuickslotKeyMappedMan::DefaultQuickslotKeyMap
    PatchJmp(0x0072B8E6, &DefaultQuickslotKeyMap_cave);
    PatchJmp(0x008CFDFD, &Restore_Array_Expanded_cave);  // 销毁  
    PatchNop(0x008CFDFD + 5, 0x008CFDFD + 6);
}