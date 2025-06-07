// Copyright Epic Games, Inc. All Rights Reserved.

#include "XLua.h"

// C++ 컴파일러가 C 소스 코드를 올바르게 처리하도록 설정
#if PLATFORM_WINDOWS
#pragma warning(push)
#pragma warning(disable: 4706) // assignment within conditional expression
#pragma warning(disable: 4310)
#pragma warning(disable: 4334)
#pragma warning(disable: 4002)
#endif

#ifdef check
#undef check
#endif

// Lua 소스 파일들을 여기에 포함시킵니다.
#include "ThirdParty/lua/lapi.c"
#include "ThirdParty/lua/lauxlib.c"
#include "ThirdParty/lua/lbaselib.c"
#include "ThirdParty/lua/lcode.c"
#include "ThirdParty/lua/lcorolib.c"
#include "ThirdParty/lua/lctype.c"
#include "ThirdParty/lua/ldblib.c"
#include "ThirdParty/lua/ldebug.c"
#include "ThirdParty/lua/ldo.c"
#include "ThirdParty/lua/ldump.c"
#include "ThirdParty/lua/lfunc.c"
#include "ThirdParty/lua/lgc.c"
#include "ThirdParty/lua/linit.c"
#include "ThirdParty/lua/liolib.c"
#include "ThirdParty/lua/llex.c"
#include "ThirdParty/lua/lmathlib.c"
#include "ThirdParty/lua/lmem.c"
#include "ThirdParty/lua/loadlib.c"
#include "ThirdParty/lua/lobject.c"
#include "ThirdParty/lua/lopcodes.c"
#include "ThirdParty/lua/loslib.c"
#include "ThirdParty/lua/lparser.c"
#include "ThirdParty/lua/lstate.c"
#include "ThirdParty/lua/lstring.c"
#include "ThirdParty/lua/lstrlib.c"
#include "ThirdParty/lua/ltable.c"
#include "ThirdParty/lua/ltablib.c"
#include "ThirdParty/lua/ltm.c"
#include "ThirdParty/lua/lundump.c"
#include "ThirdParty/lua/lutf8lib.c"
#include "ThirdParty/lua/lvm.c"
#include "ThirdParty/lua/lzio.c"

#if PLATFORM_WINDOWS
#pragma warning(pop)
#endif

#define LOCTEXT_NAMESPACE "FXLuaModule"

void FXLuaModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("XLua Module has been loaded!"));
}

void FXLuaModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXLuaModule, XLua)