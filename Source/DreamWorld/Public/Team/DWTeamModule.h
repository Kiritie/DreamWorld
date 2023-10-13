// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DWTeamModuleTypes.h"
#include "GameFramework/Actor.h"
#include "Voxel/VoxelModule.h"
#include "DWTeamModule.generated.h"

class IDWTeamAgentInterface;
/**
 * 体素模块
 */
UCLASS()
class DREAMWORLD_API ADWTeamModule : public AModuleBase, public ISaveDataInterface
{
	GENERATED_BODY()
		
	GENERATED_MODULE(ADWTeamModule)

public:
	// Sets default values for this actor's properties
	ADWTeamModule();

	~ADWTeamModule();

	//////////////////////////////////////////////////////////////////////////
	/// Module
public:
#if WITH_EDITOR
	virtual void OnGenerate() override;

	virtual void OnDestroy() override;
#endif

	virtual void OnInitialize_Implementation() override;

	virtual void OnPreparatory_Implementation(EPhase InPhase) override;

	virtual void OnRefresh_Implementation(float DeltaSeconds) override;

	virtual void OnPause_Implementation() override;

	virtual void OnUnPause_Implementation() override;

	virtual void OnTermination_Implementation(EPhase InPhase) override;

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData(bool bRefresh) override;

	virtual void UnloadData(EPhase InPhase) override;

	//////////////////////////////////////////////////////////////////////////
	// Team
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Team")
	TMap<FName, FDWTeamSaveData> TeamDatas;

public:
	virtual bool IsExistTeam(const FName& InTeamID) const;

	virtual bool CreateTeam(IDWTeamAgentInterface* InCaptain, FName InTeamName = NAME_None, FString InTeamDetail = TEXT(""));

	virtual bool DissolveTeam(const FName& InTeamID, IDWTeamAgentInterface* InCaptain = nullptr);

	virtual FDWTeamSaveData& GetTeamData(const FName& InTeamID);
};
