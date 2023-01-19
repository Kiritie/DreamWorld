// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWTeamModuleTypes.h"
#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "Voxel/VoxelModule.h"
#include "DWTeamModule.generated.h"

/**
 * 体素模块
 */
UCLASS()
class DREAMWORLD_API ADWTeamModule : public AModuleBase
{
	GENERATED_BODY()
		
	MODULE_INSTANCE_DECLARE(ADWTeamModule)

public:
	// Sets default values for this actor's properties
	ADWTeamModule();

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

	virtual void OnTermination_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	// Team
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Team")
	TMap<FName, FDWTeamData> TeamMap;

public:
	virtual bool IsExistTeam(const FName& InTeamID) const;

	virtual bool CreateTeam(ADWCharacter* InCaptain, FName InTeamName = NAME_None, FString InTeamDetail = TEXT(""));

	virtual bool DissolveTeam(const FName& InTeamID, ADWCharacter* InCaptain = nullptr);

	virtual FDWTeamData* GetTeamData(const FName& InTeamID);
};
