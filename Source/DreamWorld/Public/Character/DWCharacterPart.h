// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Global/DWGlobalTypes.h"
#include "Components/BoxComponent.h"
#include "Voxel/VoxelModuleTypes.h"

#include "DWCharacterPart.generated.h"

class UVoxel;
/**
 * 角色部位组件
 */
UCLASS()
class DREAMWORLD_API UDWCharacterPart : public UBoxComponent
{
	GENERATED_BODY()
	
public:
	UDWCharacterPart(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void OnHitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult);

	virtual void OnEnterVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult);

	virtual void OnStayVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult);

	virtual void OnExitVoxel(UVoxel& InVoxel, const FVoxelHitResult& InHitResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EDWCharacterPartType CharacterPartType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVoxelItem LastOverlapVoxel;

public:
	UFUNCTION(BlueprintPure)
	EDWCharacterPartType GetCharacterPartType() const { return CharacterPartType; }

	UFUNCTION(BlueprintCallable)
	void SetCharacterPartType(EDWCharacterPartType InCharacterPartType) { this->CharacterPartType = InCharacterPartType; }

	UFUNCTION(BlueprintPure)
	FVoxelItem& GetLastOverlapVoxel() { return LastOverlapVoxel; }

	UFUNCTION(BlueprintPure)
	ADWCharacter* GetOwnerCharacter() const;
};
