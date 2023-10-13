// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/SaveGameModuleTypes.h"

#include "DWTeamModuleTypes.generated.h"

class IDWTeamAgentInterface;

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWTeamSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FName ID;

	UPROPERTY(BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Detail;

	UPROPERTY()
	FGuid Captain;

	UPROPERTY()
	TArray<FGuid> Members;
	
	static FDWTeamSaveData Empty;

	FORCEINLINE FDWTeamSaveData()
	{
		ID = NAME_None;
		Name = NAME_None;
		Detail = TEXT("");
	}

	void AddMember(IDWTeamAgentInterface* InMember);

	void RemoveMember(IDWTeamAgentInterface* InMember);

	void DissolveTeam();

	TArray<IDWTeamAgentInterface*> GetMembers(IDWTeamAgentInterface* InMember = nullptr);

	FORCEINLINE int32 GetNumMember() const
	{
		return Members.Num();
	}

	FORCEINLINE bool IsCaptain(IDWTeamAgentInterface* InMember) const;

	FORCEINLINE bool IsValid() const override
	{
		return !ID.IsNone();
	}
};

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWTeamModuleSaveData : public FSaveData
{
	GENERATED_BODY()

public:
	FORCEINLINE FDWTeamModuleSaveData()
	{
		TeamDatas = TMap<FName, FDWTeamSaveData>();
	}

public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FDWTeamSaveData> TeamDatas;

public:
	virtual void MakeSaved() override
	{
		Super::MakeSaved();
		for(auto& Iter : TeamDatas)
		{
			Iter.Value.MakeSaved();
		}
	}
};
