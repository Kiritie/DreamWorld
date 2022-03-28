// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DWHelper.generated.h"

UENUM(BlueprintType)
enum class EDebugType : uint8
{
	Screen,
	Console,
	Both
};

UCLASS()
class DREAMWORLD_API UDWHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	//////////////////////////////////////////////////////////////////////////
	// References
private:
	static class ADWGameMode* CurrentGameMode;

	static class ADWGameState* CurrentGameState;

	static class UDWGameInstance* CurrentGameInstance;

	static class ADWPlayerController* CurrentPlayerController;

	static class ADWPlayerController* CurrentVoxelModule;
	
public:
	UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext"), Category = "DWHelper")
	static class ADWGameMode* GetGameMode(const UObject* InWorldContext);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext"), Category = "DWHelper")
	static class ADWGameState* GetGameState(const UObject* InWorldContext);
	
	UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext"), Category = "DWHelper")
	static class UDWGameInstance* GetGameInstance(const UObject* InWorldContext);
		
	UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext"), Category = "DWHelper")
	static class ADWPlayerCharacter* GetPlayerCharacter(const UObject* InWorldContext);
	
	UFUNCTION(BlueprintPure, meta = (WorldContext = "InWorldContext"), Category = "DWHelper")
	static class ADWPlayerController* GetPlayerController(const UObject* InWorldContext);

	UFUNCTION(BlueprintPure, Category = "DWHelper")
	static class ADWMainModule* GetMainModule();
	
	//////////////////////////////////////////////////////////////////////////
	// Race
public:
	static FVitalityRaceData RandomVitalityRaceData();
	
	static FCharacterRaceData RandomCharacterRaceData();

	//////////////////////////////////////////////////////////////////////////
	// Trace
public:
	UFUNCTION(BlueprintPure, Category = "DWHelper")
	static ETraceTypeQuery GetGameTrace(EGameTraceType InGameTraceType);
};
