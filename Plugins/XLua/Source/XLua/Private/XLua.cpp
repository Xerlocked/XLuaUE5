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

extern "C"
{
#include "ThirdParty/lua/lua.h"
#include "ThirdParty/lua/lauxlib.h"
#include "ThirdParty/lua/lualib.h"
}

static int LuaPrint_UE(lua_State* L)
{
	// Lua 스택에 있는 모든 인자를 문자열로 만들어서 합칩니다.
	FString FinalString;
	int NumArgs = lua_gettop(L); // 'print' 함수에 전달된 인자의 개수
	for (int i = 1; i <= NumArgs; i++)
	{
		// luaL_tolstring 함수는 스택의 값을 문자열로 변환하고 그 결과를 스택에 새로 push합니다.
		const char* Str = luaL_tolstring(L, i, nullptr);
		if (Str)
		{
			FinalString += UTF8_TO_TCHAR(Str);
			// 인자 사이에 탭(tab)을 추가하여 구분합니다.
			if (i < NumArgs)
			{
				FinalString += TEXT("\t");
			}
		}
	}
    
	// 최종적으로 합쳐진 문자열을 UE_LOG로 출력합니다.
	// [LogTemp] 카테고리에 'LUA: ' 라는 접두사를 붙여 일반 로그(Log) 레벨로 출력합니다.
	UE_LOG(LogTemp, Log, TEXT("LUA: %s"), *FinalString);

	// 이 함수는 Lua에 아무런 값을 리턴하지 않으므로 0을 리턴합니다.
	return 0;
}


void FXLuaModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("XLua Module has been loaded!"));
	
	lua_State* L = luaL_newstate();
	if (L)
	{
		luaL_openlibs(L);
		
		lua_pushcfunction(L, LuaPrint_UE);
		lua_setglobal(L, "print");

		UE_LOG(LogTemp, Log, TEXT("Overwrote Lua's print function. Executing Lua script..."));

		// 테스트를 위해 여러 인자를 넘겨봅니다.
		int Result = luaL_dostring(L, "print('>>>> Hello from Lua, running inside UE5!', 123, true, nil)");

		if (Result != LUA_OK)
		{
			const char* ErrorMsg = lua_tostring(L, -1);
			UE_LOG(LogTemp, Error, TEXT("Lua Error: %s"), UTF8_TO_TCHAR(ErrorMsg));
		}
        
		lua_close(L);
	}
}

void FXLuaModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXLuaModule, XLua)