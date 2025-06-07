// copyright 2025 @xerlocked

#include "XLuaVirtualMachine.h"
#include "Misc/FileHelper.h"

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

static int GenericUFunctionCall_Thunk(lua_State* L)
{
	UObject* TargetObject = static_cast<UObject*>(lua_touserdata(L, lua_upvalueindex(1)));
	UFunction* FunctionToCall = static_cast<UFunction*>(lua_touserdata(L, lua_upvalueindex(2)));

	if (TargetObject == nullptr || FunctionToCall == nullptr)
	{
		return 0;
	}

	void* Params = FMemory_Alloca(FunctionToCall->ParmsSize);
	FMemory::Memzero(Params, FunctionToCall->ParmsSize);

	int LuaStackIndex = 2;
	for (TFieldIterator<FProperty> Iterator(FunctionToCall); Iterator; ++Iterator)
	{
		FProperty* Prop = *Iterator;
		if (Prop->HasAnyPropertyFlags(CPF_Parm) && !Prop->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm))
		{
			if (FStrProperty* StrProp = CastField<FStrProperty>(Prop))
			{
				if (lua_type(L, LuaStackIndex) == LUA_TSTRING)
				{
					const char* LuaString = lua_tostring(L, LuaStackIndex);
					FString* ParamPtr = StrProp->ContainerPtrToValuePtr<FString>(Params);
					*ParamPtr = FString(UTF8_TO_TCHAR(LuaString));
				}
			}

			// TODO: FFloatProperty, FIntProperty, ...

			LuaStackIndex++;
		}
	}
	
	TargetObject->ProcessEvent(FunctionToCall, Params);
	
	int NumReturnValues = 0;
	for (TFieldIterator<FProperty> Iterator(FunctionToCall); Iterator; ++Iterator)
	{
		FProperty* Prop = *Iterator;
		if (Prop->HasAnyPropertyFlags(CPF_ReturnParm))
		{
			// FString
			if (FStrProperty* StrProp = CastField<FStrProperty>(Prop))
			{
				FString& ReturnValue = *StrProp->ContainerPtrToValuePtr<FString>(Params);
				lua_pushstring(L, TCHAR_TO_UTF8(*ReturnValue));
				NumReturnValues = 1;
				break;
			}
			
			// FVector
			if (FStructProperty* StructProp = CastField<FStructProperty>(Prop))
			{
				if (StructProp->Struct == TBaseStructure<FVector>::Get())
				{
					FVector& ReturnValue = *StructProp->ContainerPtrToValuePtr<FVector>(Params);
					lua_newtable(L);
					lua_pushnumber(L, ReturnValue.X);
					lua_setfield(L, -2, "x");
					lua_pushnumber(L, ReturnValue.Y);
					lua_setfield(L, -2, "y");
					lua_pushnumber(L, ReturnValue.Z);
					lua_setfield(L, -2, "z");
					NumReturnValues = 1;
					break;
				}
			}
		}
	}
	
	return NumReturnValues;
}

static int UObject__index(lua_State* L)
{
	UObject** UObjectPtr = static_cast<UObject**>(lua_touserdata(L, 1));
	const char* Key = lua_tostring(L, 2);

	if (UObjectPtr == nullptr || *UObjectPtr == nullptr || Key == nullptr)
	{
		lua_pushnil(L);
		return 1;
	}

	UObject* TargetObject = *UObjectPtr;
	UClass* TargetClass = TargetObject->GetClass();

	UFunction* FoundFunction = TargetClass->FindFunctionByName(FName(Key));
	
	if (IsValid(FoundFunction))
	{
		lua_pushlightuserdata(L, TargetObject);
		lua_pushlightuserdata(L, FoundFunction);

		lua_pushcclosure(L, GenericUFunctionCall_Thunk, 2);
	}
	else
	{
		lua_pushnil(L);
	}
	
	return 1;
}

XLuaVirtualMachine::XLuaVirtualMachine()
{
	L = luaL_newstate();
	if (L)
	{
		luaL_openlibs(L);

		lua_pushcfunction(L, &RunPrint);
		lua_setglobal(L, "print");

		if (luaL_newmetatable(L, "UObjectMeta"))
		{
			lua_pushcfunction(L, UObject__index);
			lua_setfield(L, -2, "__index");
		}
		lua_pop(L, 1);
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
		if (ErrorMsg)
		{
			FString SafeErrorMessage(UTF8_TO_TCHAR(ErrorMsg));
			UE_LOG(LogTemp, Error, TEXT("Lua Error: %s"), *SafeErrorMessage);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Lua Error: Unknown error (message is null)."));
		}
        
		lua_pop(L, 1);
		return false;
	}
	return true;
}

bool XLuaVirtualMachine::RunFile(const FString& FilePath) const
{
	FString ScriptCode;
	
	if (!FFileHelper::LoadFileToString(ScriptCode, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load lua file: %s"), *FilePath);
		return false;
	}
	return RunString(ScriptCode);
}

void XLuaVirtualMachine::BindUObject(UObject* InObject) const
{
	if (!L || !InObject)
	{
		lua_pushnil(L);
		return;
	}

	void* UserData = lua_newuserdatauv(L, sizeof(UObject*), 0);
	*static_cast<UObject**>(UserData) = InObject;

	luaL_setmetatable(L, "UObjectMeta");
}
