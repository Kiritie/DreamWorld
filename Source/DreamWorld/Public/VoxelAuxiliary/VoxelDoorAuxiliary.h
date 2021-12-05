// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld/DreamWorld.h"
#include "VoxelAuxiliary/VoxelVirtualAuxiliary.h"
#include "VoxelDoorAuxiliary.generated.h"

class UVoxel;

/**
 * �Ÿ�����
 */
UCLASS()
class DREAMWORLD_API AVoxelDoorAuxiliary : public AVoxelVirtualAuxiliary
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AVoxelDoorAuxiliary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:	
	virtual void Initialize(AChunk* InOwnerChunk, FIndex InVoxelIndex) override;

	virtual bool OnInteract(IInteraction* InTrigger, EInteractOption InInteractOption) override;

	virtual void OpenDoor();

	virtual void CloseDoor();
};
