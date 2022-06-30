// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/DWVoxelModule.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Procedure_ArchiveChoosing.h"
#include "Procedure/Procedure_ArchiveCreating.h"
#include "Procedure/Procedure_Loading.h"
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
	if(UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Starting>()
		|| UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_ArchiveChoosing>()
		|| UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_ArchiveCreating>()
		|| UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Loading>()
		|| UProcedureModuleBPLibrary::IsCurrentProcedureClass<UProcedure_Playing>())
	{
		Super::OnRefresh_Implementation(DeltaSeconds);
	}
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

void ADWVoxelModule::OnWorldStateChanged()
{
	Super::OnWorldStateChanged();
}

void ADWVoxelModule::LoadData(FSaveData* InSaveData)
{
	Super::LoadData(InSaveData);
}

FSaveData* ADWVoxelModule::ToData()
{
	for(auto Iter : ChunkMap)
	{
		if(Iter.Value)
		{
			GetWorldData<FDWVoxelWorldSaveData>()->SetChunkData(Iter.Key, USaveGameModuleBPLibrary::ObjectToDataRef<FDWVoxelChunkSaveData>(Iter.Value));
		}
	}
	return WorldData;
}

void ADWVoxelModule::UnloadData()
{
	Super::UnloadData();
}

void ADWVoxelModule::GeneratePreviews()
{
	Super::GeneratePreviews();
}

void ADWVoxelModule::GenerateTerrain()
{
	Super::GenerateTerrain();
}

void ADWVoxelModule::GenerateChunks(FIndex InIndex)
{
	Super::GenerateChunks(InIndex);

	if (ChunkSpawnQueue.Num() > 0) return;

	if(BoundsMesh != nullptr)
	{
		BoundsMesh->SetRelativeLocation(UVoxelModuleBPLibrary::ChunkIndexToLocation(InIndex));
		BoundsMesh->SetRelativeScale3D(FVector(GetWorldLength() * GetWorldData()->BlockSize * 0.01f, GetWorldLength() * GetWorldData()->BlockSize * 0.01f, 15.f));
	}
}

void ADWVoxelModule::BuildChunkMap(AVoxelChunk* InChunk)
{
	if (!InChunk || !ChunkMap.Contains(InChunk->GetIndex())) return;

	if (GetWorldData<FDWVoxelWorldSaveData>()->IsExistChunkData(InChunk->GetIndex()))
	{
		InChunk->LoadData(&GetWorldData<FDWVoxelWorldSaveData>()->GetChunkData(InChunk->GetIndex()));
	}
	else
	{
		InChunk->BuildMap();
	}

	AddToMapGenerateQueue(InChunk);
}

void ADWVoxelModule::GenerateChunkMap(AVoxelChunk* InChunk)
{
	Super::GenerateChunkMap(InChunk);
}

void ADWVoxelModule::GenerateChunk(AVoxelChunk* InChunk)
{
	Super::GenerateChunk(InChunk);
}

void ADWVoxelModule::DestroyChunk(AVoxelChunk* InChunk)
{
	if(!InChunk || !ChunkMap.Contains(InChunk->GetIndex())) return;

	GetWorldData<FDWVoxelWorldSaveData>()->SetChunkData(InChunk->GetIndex(), USaveGameModuleBPLibrary::ObjectToDataRef<FDWVoxelChunkSaveData>(InChunk));
	
	Super::DestroyChunk(InChunk);
}

AVoxelChunk* ADWVoxelModule::SpawnChunk(FIndex InIndex, bool bAddToQueue)
{
	return Super::SpawnChunk(InIndex, bAddToQueue);
}

bool ADWVoxelModule::ChunkTraceSingle(AVoxelChunk* InChunk, float InRadius, float InHalfHeight, FHitResult& OutHitResult)
{
	FVector rayStart = InChunk->GetActorLocation() + FVector(FMath::FRandRange(1.f, GetWorldData()->ChunkSize - 1), FMath::FRandRange(1.f, GetWorldData()->ChunkSize), GetWorldData()->ChunkSize) * GetWorldData()->BlockSize;
	rayStart.X = ((int32)(rayStart.X / GetWorldData()->BlockSize) + 0.5f) * GetWorldData()->BlockSize;
	rayStart.Y = ((int32)(rayStart.Y / GetWorldData()->BlockSize) + 0.5f) * GetWorldData()->BlockSize;
	FVector rayEnd = rayStart + FVector::DownVector * GetWorldData()->GetChunkLength();
	return ChunkTraceSingle(rayStart, rayEnd, InRadius * 0.95f, InHalfHeight * 0.95f, OutHitResult);
}

bool ADWVoxelModule::ChunkTraceSingle(FVector RayStart, FVector RayEnd, float InRadius, float InHalfHeight, FHitResult& OutHitResult)
{
	if (UKismetSystemLibrary::CapsuleTraceSingle(this, RayStart, RayEnd, InRadius, InHalfHeight, UDWHelper::GetGameTrace(EDWGameTraceType::Chunk), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true))
		return OutHitResult.ImpactPoint.Z > 0;
	return false;
}

bool ADWVoxelModule::VoxelTraceSingle(const FVoxelItem& InVoxelItem, FVector InPoint, FHitResult& OutHitResult)
{
	FVector size = InVoxelItem.GetData<UVoxelData>().GetCeilRange(InVoxelItem.Rotation, InVoxelItem.Scale) * GetWorldData()->BlockSize * 0.5f;
	return UKismetSystemLibrary::BoxTraceSingle(this, InPoint + size, InPoint + size, size * 0.95f, FRotator::ZeroRotator, UDWHelper::GetGameTrace(EDWGameTraceType::Voxel), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true);
}
