// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterModule.h"
#include "Common/DWCommonTypes.h"

#include "DWCharacterModule.generated.h"

UCLASS()
class DREAMWORLD_API UDWCharacterModule : public UCharacterModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(UDWCharacterModule)

public:	
	// ParamSets default values for this actor's properties
	UDWCharacterModule();

	~UDWCharacterModule();
	
	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize() override;

	virtual void OnPreparatory(EPhase InPhase) override;

	virtual void OnRefresh(float DeltaSeconds, bool bInEditor) override;

	virtual void OnPause() override;

	virtual void OnUnPause() override;

	virtual void OnTermination(EPhase InPhase) override;

	//////////////////////////////////////////////////////////////////////////
	/// Player
protected:
	UPROPERTY(EditAnywhere, Category = "Player")
	FDWPlayerBasicSaveData PlayerBasicData;
	
public:
	UFUNCTION(BlueprintPure)
	FDWPlayerBasicSaveData& GetPlayerBasicData();
};
