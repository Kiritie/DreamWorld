// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelAuxiliary/VoxelAuxiliary.h"

#include "Interaction/Components/InteractionComponent.h"
#include "Voxel/Voxel.h"
#include "World/Chunk.h"

// Sets default values
AVoxelAuxiliary::AVoxelAuxiliary()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Interaction = CreateDefaultSubobject<UInteractionComponent>(FName("Interaction"));
	Interaction->SetupAttachment(RootComponent);
	Interaction->SetRelativeLocation(FVector(0, 0, 0));

	VoxelIndex = FIndex::ZeroIndex;
}

// Called when the game starts or when spawned
void AVoxelAuxiliary::BeginPlay()
{
	Super::BeginPlay();

}

void AVoxelAuxiliary::Initialize(AChunk* InOwnerChunk, FIndex InVoxelIndex)
{
	VoxelIndex = InVoxelIndex;
}

bool AVoxelAuxiliary::OnInteract(IInteraction* InTrigger, EInteractOption InInteractOption)
{
	if (!InTrigger) return false;

	if(InteractOptions.Contains(InInteractOption))
	{
		// switch (InInteractOption)
		// {
		// 	default: break;
		// }
		return true;
	}
	return false;
}

FVoxelItem& AVoxelAuxiliary::GetVoxelItem() const
{
	if(OwnerChunk)
	{
		return OwnerChunk->GetVoxelItem(VoxelIndex);
	}
	return FVoxelItem::EmptyVoxel;
}
