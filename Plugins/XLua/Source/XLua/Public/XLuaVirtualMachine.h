// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
extern "C"
{
#include "ThirdParty/lua/lua.h"
#include "ThirdParty/lua/lauxlib.h"
#include "ThirdParty/lua/lualib.h"
}
#include "CoreMinimal.h"

struct lua_State;

class XLUA_API XLuaVirtualMachine
{
public:
	XLuaVirtualMachine();
	~XLuaVirtualMachine();
	
	bool RunString(const FString& Code) const;

	bool RunFile(const FString& FilePath) const;

	void BindUObject(UObject* InObject) const;

	lua_State* GetLuaState() const { return L; }

	XLuaVirtualMachine(const XLuaVirtualMachine&) = delete;
	XLuaVirtualMachine& operator=(const XLuaVirtualMachine&) = delete;
	
private:
	lua_State* L;
};
