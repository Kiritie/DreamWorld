// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "GameFramework/Actor.h"
#include "Voxel/VoxelModule.h"
#include "DWTeamModule.generated.h"

/**
 * 体素模块
 */
UCLASS(hidecategories = (Tick, Replication, Rendering, Collision, Actor, Input, LOD, Cooking, Hidden, WorldPartition, Hlod))
class WHFRAMEWORK_API ADWTeamModule : public AVoxelModule
{
	GENERATED_BODY()
	
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

	virtual void OnPreparatory_Implementation() override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	virtual void OnTermination_Implementation() override;

	//////////////////////////////////////////////////////////////////////////
	// Team
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Team")
	TMap<FName, FTeamData> TeamMap;

public:
	virtual bool IsExistTeam(const FName& InTeamID) const;

	virtual bool CreateTeam(ADWCharacter* InCaptain, FName InTeamName = NAME_None, FString InTeamDetail = TEXT(""));

	virtual bool DissolveTeam(const FName& InTeamID, ADWCharacter* InCaptain = nullptr);

	virtual FTeamData* GetTeamData(const FName& InTeamID);
};
