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
class DREAMWORLD_API UDWTeamModule : public UModuleBase
{
	GENERATED_BODY()
		
	GENERATED_MODULE(UDWTeamModule)

public:
	// Sets default values for this actor's properties
	UDWTeamModule();

	~UDWTeamModule();

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

protected:
	virtual void LoadData(FSaveData* InSaveData, EPhase InPhase) override;

	virtual FSaveData* ToData() override;

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
