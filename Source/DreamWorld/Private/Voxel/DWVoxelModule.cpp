// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/DWVoxelModule.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/DWGameState.h"
#include "Gameplay/DWPlayerController.h"
#include "Global/GlobalBPLibrary.h"
#include "Procedure/ProcedureModuleBPLibrary.h"
#include "Procedure/Archive/Procedure_ArchiveChoosing.h"
#include "Procedure/Archive/Procedure_ArchiveCreating.h"
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
#include "UObject/ConstructorHelpers.h"
	
IMPLEMENTATION_MODULE(ADWVoxelModule)

// Sets default values
ADWVoxelModule::ADWVoxelModule()
{
	ChunkSpawnClass = ADWVoxelChunk::StaticClass();

	BoundsMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoundsMesh"));
	BoundsMesh->SetRelativeScale3D(FVector::ZeroVector);
	BoundsMesh->SetRelativeRotation(FRotator(0, 0, 0));
	BoundsMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WorldBasicData.ChunkMaterials.Empty();
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SolidMatFinder(TEXT("MaterialInstanceConstant'/Game/Materials/Voxel/M_DW_Voxels_Solid.M_DW_Voxels_Solid'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> UnlitSolidMatFinder(TEXT("MaterialInstanceConstant'/Game/Materials/Voxel/M_DW_Voxels_Solid_Unlit.M_DW_Voxels_Solid_Unlit'"));
	if(SolidMatFinder.Succeeded() && UnlitSolidMatFinder.Succeeded())
	{
		WorldBasicData.ChunkMaterials.Add(FVoxelChunkMaterial(SolidMatFinder.Object, UnlitSolidMatFinder.Object, FVector2D(0.0625f, 0.5f)));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SemiTransparentMatFinder(TEXT("MaterialInstanceConstant'/Game/Materials/Voxel/M_DW_Voxels_SemiTransparent.M_DW_Voxels_SemiTransparent'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> UnlitSemiTransparentMatFinder(TEXT("MaterialInstanceConstant'/Game/Materials/Voxel/M_DW_Voxels_SemiTransparent_Unlit.M_DW_Voxels_SemiTransparent_Unlit'"));
	if(SemiTransparentMatFinder.Succeeded() && UnlitSemiTransparentMatFinder.Succeeded())
	{
		WorldBasicData.ChunkMaterials.Add(FVoxelChunkMaterial(SemiTransparentMatFinder.Object, UnlitSemiTransparentMatFinder.Object, FVector2D(0.0625f, 0.5f)));
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TransparentMatFinder(TEXT("MaterialInstanceConstant'/Game/Materials/Voxel/M_DW_Voxels_Transparent.M_DW_Voxels_Transparent'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> UnlitTransparentMatFinder(TEXT("MaterialInstanceConstant'/Game/Materials/Voxel/M_DW_Voxels_Transparent_Unlit.M_DW_Voxels_Transparent_Unlit'"));
	if(TransparentMatFinder.Succeeded() && UnlitTransparentMatFinder.Succeeded())
	{
		WorldBasicData.ChunkMaterials.Add(FVoxelChunkMaterial(TransparentMatFinder.Object, UnlitTransparentMatFinder.Object, FVector2D(0.0625f, 0.5f)));
	}
}

ADWVoxelModule::~ADWVoxelModule()
{
	TERMINATION_MODULE(ADWVoxelModule)
}

#if WITH_EDITOR
void ADWVoxelModule::OnGenerate()
{
	Super::OnGenerate();
}

void ADWVoxelModule::OnDestroy()
{
	Super::OnDestroy();
}
#endif

void ADWVoxelModule::OnInitialize_Implementation()
{
	Super::OnInitialize_Implementation();
}

void ADWVoxelModule::OnPreparatory_Implementation(EPhase InPhase)
{
	Super::OnPreparatory_Implementation(InPhase);
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

void ADWVoxelModule::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);

	auto& SaveData = InSaveData->CastRef<FDWVoxelWorldSaveData>();

	switch(InPhase)
	{
		case EPhase::Primary:
		{
			WorldData->CastRef<FDWVoxelWorldSaveData>().ChunkDatas = SaveData.ChunkDatas;
			break;
		}
		case EPhase::Lesser:
		case EPhase::Final:
		{
			break;
		}
	}
}

FSaveData* ADWVoxelModule::ToData(bool bRefresh)
{
	return Super::ToData(bRefresh);
}

void ADWVoxelModule::UnloadData(EPhase InPhase)
{
	Super::UnloadData(InPhase);
}

FVoxelWorldSaveData* ADWVoxelModule::NewData(bool bInheritBasicData) const
{
	static FDWVoxelWorldSaveData* NewWorldData;
	NewWorldData = !bInheritBasicData ? new FDWVoxelWorldSaveData() : new FDWVoxelWorldSaveData(WorldBasicData);
	return NewWorldData;
}

void ADWVoxelModule::OnWorldStateChanged()
{
	Super::OnWorldStateChanged();
}

void ADWVoxelModule::GenerateWorld()
{
	Super::GenerateWorld();

	if(BoundsMesh && LastGenerateIndex != Index_Empty)
	{
		BoundsMesh->SetRelativeLocation(AVoxelModule::ChunkIndexToLocation(FIndex(LastGenerateIndex.X, LastGenerateIndex.Y, 0)));
		BoundsMesh->SetRelativeScale3D(FVector(WorldData->GetWorldRealSize().X * WorldData->BlockSize * 0.01f, WorldData->GetWorldRealSize().Y * WorldData->BlockSize * 0.01f, 15.f));
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

ECollisionChannel ADWVoxelModule::GetChunkTraceType() const
{
	return (ECollisionChannel)EDWGameTraceType::Chunk;
}

ECollisionChannel ADWVoxelModule::GetVoxelTraceType() const
{
	return (ECollisionChannel)EDWGameTraceType::Voxel;
}
