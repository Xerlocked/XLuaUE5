//  copyright 2025 @xerlocked

#include "XLuaComponent.h"

#include <ThirdParty/lua/lua.h>

#include "XLuaVirtualMachine.h"
#include "Misc/Paths.h"

UXLuaComponent::UXLuaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UXLuaComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (ScriptFilePath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("XLuaComponent: ScriptFile is not set."));
		return;
	}
	
	// Make full path based on content folder.
	const FString FullPath = FPaths::ProjectContentDir() / ScriptFilePath;

	UE_LOG(LogTemp, Log, TEXT("XLuaComponent: Attempting to run Lua script: %s"), *FullPath);
	
	VMInstance = MakeShared<XLuaVirtualMachine>();

	if (AActor* Owner = GetOwner())
	{
		VMInstance->BindUObject(Owner);
		lua_setglobal(VMInstance->GetLuaState(), "self");
	}
	
	VMInstance->RunFile(FullPath);
}

void UXLuaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	VMInstance.Reset();

	UE_LOG(LogTemp, Log, TEXT("XLuaComponent: EndPlay called, Lua VM instance released."));

	Super::EndPlay(EndPlayReason);
}
