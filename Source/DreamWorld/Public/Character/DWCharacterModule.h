// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterModule.h"

#include "DWCharacterModule.generated.h"

UCLASS()
class DREAMWORLD_API ADWCharacterModule : public ACharacterModule
{
	GENERATED_BODY()
		
	GENERATED_MODULE(ADWCharacterModule)

public:	
	// ParamSets default values for this actor's properties
	ADWCharacterModule();
	
	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate_Implementation() override;

	virtual void OnDestroy_Implementation() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	/// Player
protected:
	UPROPERTY(EditAnywhere, Category = "Player")
	FDWPlayerBasicSaveData PlayerBasicData;
public:
	UFUNCTION(BlueprintPure)
	FDWPlayerBasicSaveData& GetPlayerBasicData();
};
