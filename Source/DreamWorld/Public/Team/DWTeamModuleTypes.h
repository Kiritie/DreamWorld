// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DWTeamModuleTypes.generated.h"

class ADWCharacter;

USTRUCT(BlueprintType)
struct DREAMWORLD_API FDWTeamData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FName ID;

	UPROPERTY(BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Detail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADWCharacter* Captain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ADWCharacter*> Members;
	
	static FDWTeamData Empty;

	FORCEINLINE FDWTeamData()
	{
		ID = NAME_None;
		Name = NAME_None;
		Detail = TEXT("");
		Captain = nullptr;
		Members = TArray<ADWCharacter*>();
	}

	void AddMember(ADWCharacter* InMember);

	void RemoveMember(ADWCharacter* InMember);

	void DissolveTeam();

	TArray<ADWCharacter*> GetMembers(ADWCharacter* InMember = nullptr);

	FORCEINLINE int32 GetNumMember() const
	{
		return Members.Num();
	}

	FORCEINLINE bool IsCaptain(ADWCharacter* InMember) const
	{
		return Captain == nullptr || Captain == InMember;
	}

	FORCEINLINE bool IsValid() const
	{
		return !ID.IsNone();
	}
};
