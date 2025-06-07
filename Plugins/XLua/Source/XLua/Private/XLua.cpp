// Copyright Epic Games, Inc. All Rights Reserved.

#include "XLua.h"

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