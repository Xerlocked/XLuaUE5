// copyright 2025 @xerlocked

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XLuaComponent.generated.h"

class XLuaVirtualMachine;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XLUA_API UXLuaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UXLuaComponent();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lua", meta = (ToolTip = "Excute Lua script file path. (Based on Content Folder)"))
	FString ScriptFilePath;
	
private:
	TSharedPtr<XLuaVirtualMachine> VMInstance;
};
