// copyright 2025 @xerlocked

#include "XLuaVirtualMachine.h"
#include "Misc/FileHelper.h"

extern "C"
{
#include "ThirdParty/lua/lua.h"
#include "ThirdParty/lua/lauxlib.h"
#include "ThirdParty/lua/lualib.h"
}

static int RunPrint(lua_State* L)
{
	FString FinalString;
	int NumArgs = lua_gettop(L);
	for (int i = 1; i <= NumArgs; i++)
	{
		const char* Str = luaL_tolstring(L, i, nullptr);
		if (Str)
		{
			FinalString += UTF8_TO_TCHAR(Str);

			if (i < NumArgs)
			{
				FinalString += TEXT("\t");
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("LUA: %s"), *FinalString);

	return 0;
}


XLuaVirtualMachine::XLuaVirtualMachine()
{
	L = luaL_newstate();
	if (L)
	{
		luaL_openlibs(L);

		lua_pushcfunction(L, &RunPrint);
		lua_setglobal(L, "print");
	}
}

XLuaVirtualMachine::~XLuaVirtualMachine()
{
	if (L)
	{
		lua_close(L);
		L = nullptr;
	}
}

bool XLuaVirtualMachine::RunString(const FString& Code) const
{
	if (L == nullptr)
	{
		return false;
	}

	int Result = luaL_dostring(L, TCHAR_TO_UTF8(*Code));
	
	if (Result != LUA_OK)
	{
		const char* ErrorMsg = lua_tostring(L, -1);
		UE_LOG(LogTemp, Error, TEXT("Lua Error: %hs"), UTF8_TO_TCHAR(ErrorMsg));
		lua_pop(L, 1);
		return false;
	}
	return true;
}

bool XLuaVirtualMachine::RunFile(const FString& FilePath)
{
	FString ScriptCode;
	
	if (!FFileHelper::LoadFileToString(ScriptCode, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load lua file: %s"), *FilePath);
		return false;
	}
	return RunString(ScriptCode);
}
