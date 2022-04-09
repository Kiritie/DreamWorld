// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/DWVoxelModule.h"

#include "Character/Player/DWPlayerCharacter.h"
#include "Gameplay/DWGameState.h"
#include "SpawnPool/SpawnPoolModuleBPLibrary.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Voxel/Datas/VoxelData.h"

// Sets default values
ADWVoxelModule::ADWVoxelModule()
{
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
	switch (UGlobalBPLibrary::GetGameState<ADWGameState>(this)->GetCurrentState())
	{
		case EDWGameState::MainMenu:
		case EDWGameState::Loading:
		{
			GenerateTerrain();
			break;
		}
		case EDWGameState::Playing:
		{
			GenerateTerrain();
			break;
		}
		default: break;
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

bool ADWVoxelModule::ChangeWorldState(EVoxelWorldState InWorldState)
{
	if(Super::ChangeWorldState(InWorldState))
	{
		switch(WorldState)
		{
			case EVoxelWorldState::BasicGenerated:
			{
				if(ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>(this))
				{
					FHitResult hitResult;
					const FVector rayStart = FVector(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, GetWorldData()->ChunkHeightRange * GetWorldData()->GetChunkLength() + 500);
					const FVector rayEnd = FVector(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, 0);
					if (ChunkTraceSingle(rayStart, rayEnd, PlayerCharacter->GetRadius(), PlayerCharacter->GetHalfHeight(), hitResult))
					{
						if(PlayerCharacter->GetActorLocation().Size2D() < 1.f)
						{
							PlayerCharacter->SetActorLocation(hitResult.Location);
						}
						if(!GetWorldData<FDWVoxelWorldSaveData>()->GetArchiveData().bPreview)
						{
							if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
							{
								GameState->SetCurrentState(EDWGameState::Playing);
							}
							PlayerCharacter->Active();
						}
					}
				}
				break;
			}
			case EVoxelWorldState::FullGenerated:
			{
				break;
			}
			default: break;
		}
		return true;
	}
	return false;
}

void ADWVoxelModule::LoadData(FSaveData* InWorldData)
{
	Super::LoadData(InWorldData);

	if(!GetWorldData<FDWVoxelWorldSaveData>()->GetArchiveData().bPreview)
	{
		if(ADWGameState* GameState = UGlobalBPLibrary::GetGameState<ADWGameState>(this))
		{
			GameState->SetCurrentState(EDWGameState::Loading);
		}
		if(GetWorldData<FDWVoxelWorldSaveData>()->IsSameArchive(*static_cast<FDWVoxelWorldSaveData*>(InWorldData)))
		{
			for(auto Iter : ChunkMap)
			{
				if(Iter.Value && Iter.Value->IsGenerated())
				{
					Iter.Value->SpawnActors();
				}
			}
		}
		else
		{
			UnloadData();
		}
	}
}

FSaveData* ADWVoxelModule::ToData(bool bSaved)
{
	for(auto Iter : ChunkMap)
	{
		if(Iter.Value)
		{
			GetWorldData<FDWVoxelWorldSaveData>()->SetChunkData(Iter.Key, *static_cast<FDWVoxelChunkSaveData*>(Iter.Value->ToData(bSaved)));
		}
	}
	return WorldData;
}

void ADWVoxelModule::UnloadData(bool bPreview)
{
	Super::UnloadData(bPreview);
	
	if(!bPreview)
	{
		// TeamMap.Empty();
	}
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
		BoundsMesh->SetRelativeLocation(ChunkIndexToLocation(InIndex));
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
	if (!InChunk || !ChunkMap.Contains(InChunk->GetIndex())) return;
	
	InChunk->Generate(GetWorldData<FDWVoxelWorldSaveData>()->GetArchiveData().bPreview);
}

void ADWVoxelModule::DestroyChunk(AVoxelChunk* InChunk)
{
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
	FVector size = InVoxelItem.GetData<UVoxelData>()->GetCeilRange(InVoxelItem.Rotation, InVoxelItem.Scale) * GetWorldData()->BlockSize * 0.5f;
	return UKismetSystemLibrary::BoxTraceSingle(this, InPoint + size, InPoint + size, size * 0.95f, FRotator::ZeroRotator, UDWHelper::GetGameTrace(EDWGameTraceType::Voxel), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHitResult, true);
}
