// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/DWVoxelModule.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_ArchiveChoosing.h"
#include "Procedure/Procedure_ArchiveCreating.h"
#include "Procedure/Procedure_Initializing.h"
#include "Procedure/Procedure_Loading.h"
#include "Procedure/Procedure_Overed.h"
#include "Procedure/Procedure_Pausing.h"
#include "Procedure/Procedure_Playing.h"
#include "Procedure/Procedure_Starting.h"
#include "ReferencePool/ReferencePoolModuleBPLibrary.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/VoxelModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Datas/VoxelData.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleBPLibrary.h"
#include "Widget/Inventory/WidgetInventoryBar.h"
#include "Widget/Inventory/WidgetInventoryPanel.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

// Sets default values
ADWVoxelModule::ADWVoxelModule()
{
	ChunkSpawnClass = ADWVoxelChunk::StaticClass();

	BoundsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoundsMesh"));
	BoundsMesh->SetRelativeScale3D(FVector::ZeroVector);
	BoundsMesh->SetRelativeRotation(FRotator(0, 0, 0));
	BoundsMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

#if WITH_EDITOR
void ADWVoxelModule::OnGenerate_Implementation()
{
	Super::OnGenerate_Implementation();
}

void ADWVoxelModule::OnDestroy_Implementation()
{
	Super::OnDestroy_Implementation();
}
#endif

void ADWVoxelModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWVoxelModule::OnPreparatory_Implementation()
{
	Super::OnPreparatory_Implementation();
}

void ADWVoxelModule::OnRefresh_Implementation(float DeltaSeconds)
{
	Super::OnRefresh_Implementation(DeltaSeconds);
}

void ADWVoxelModule::OnPause_Implementation()
{
	Super::OnPause_Implementation();
}

void ADWVoxelModule::OnUnPause_Implementation()
{
	Super::OnUnPause_Implementation();
}

void ADWVoxelModule::OnTermination_Implementation()
{
	Super::OnTermination_Implementation();
}

void ADWVoxelModule::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
}

FSaveData* ADWVoxelModule::ToData()
{
	return Super::ToData();
}

void ADWVoxelModule::UnloadData(bool bForceMode)
{
	Super::UnloadData(bForceMode);
}

void ADWVoxelModule::OnWorldStateChanged()
{
	Super::OnWorldStateChanged();
}

void ADWVoxelModule::GenerateVoxels()
{
	Super::GenerateVoxels();
}

void ADWVoxelModule::GenerateWorld()
{
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Pausing>() || UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Overed>()) return;

	Super::GenerateWorld();

	if(BoundsMesh && LastGenerateIndex != FIndex(-1, -1, -1))
	{
		BoundsMesh->SetRelativeLocation(UVoxelModuleBPLibrary::ChunkIndexToLocation(LastGenerateIndex));
		BoundsMesh->SetRelativeScale3D(FVector(GetWorldLength() * WorldData->BlockSize * 0.01f, GetWorldLength() * WorldData->BlockSize * 0.01f, 15.f));
	}
}

void ADWVoxelModule::BuildChunkMap(FIndex InIndex, int32 InStage)
{
	Super::BuildChunkMap(InIndex, InStage);
}

void ADWVoxelModule::BuildChunkMesh(FIndex InIndex)
{
	Super::BuildChunkMesh(InIndex);
}

void ADWVoxelModule::GenerateChunk(FIndex InIndex)
{
	Super::GenerateChunk(InIndex);
}

void ADWVoxelModule::DestroyChunk(FIndex InIndex)
{
	Super::DestroyChunk(InIndex);
}

AVoxelChunk* ADWVoxelModule::SpawnChunk(FIndex InIndex, bool bAddToQueue)
{
	return Super::SpawnChunk(InIndex, bAddToQueue);
}

bool ADWVoxelModule::ChunkTraceSingle(AVoxelChunk* InChunk, float InRadius, float InHalfHeight, FHitResult& OutHitResult)
{
	FVector rayStart = InChunk->GetActorLocation() + FVector(FMath::FRandRange(1.f, WorldData->ChunkSize - 1), FMath::FRandRange(1.f, WorldData->ChunkSize), WorldData->ChunkSize) * WorldData->BlockSize;
	rayStart.X = ((int32)(rayStart.X / WorldData->BlockSize) + 0.5f) * WorldData->BlockSize;
	rayStart.Y = ((int32)(rayStart.Y / WorldData->BlockSize) + 0.5f) * WorldData->BlockSize;
	FVector rayEnd = rayStart + FVector::DownVector * WorldData->GetChunkLength();
	return ChunkTraceSingle(rayStart, rayEnd, InRadius * 0.95f, InHalfHeight * 0.95f, OutHitResult);
}

bool ADWVoxelModule::ChunkTraceSingle(FVector RayStart, FVector RayEnd, float InRadius, float InHalfHeight, FHitResult& OutHitResult)
{
	if (UKismetSystemLibrary::CapsuleTraceSingle(this, RayStart, RayEnd, InRadius, InHalfHeight, UDWHelper::GetGameTrace(EDWGameTraceType::Chunk), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true))
	{
		return OutHitResult.ImpactPoint.Z > 0;
	}
	return false;
}

bool ADWVoxelModule::VoxelTraceSingle(const FVoxelItem& InVoxelItem, FVector InPoint, FHitResult& OutHitResult)
{
	const FVector size = InVoxelItem.GetRange() * WorldData->BlockSize * 0.5f;
	return UKismetSystemLibrary::BoxTraceSingle(this, InPoint + size, InPoint + size, size * 0.95f, FRotator::ZeroRotator, UDWHelper::GetGameTrace(EDWGameTraceType::Voxel), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true);
}
