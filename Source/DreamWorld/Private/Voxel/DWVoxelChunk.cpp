// Fill out your copyright notice in the Description page of Project Settings.PickUp/


#include "Voxel/DWVoxelChunk.h"

#include "Ability/Character/CharacterAssetBase.h"
#include "Ability/Vitality/VitalityAssetBase.h"
#include "Character/DWCharacter.h"
#include "Character/Base/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Main/MainModule.h"
#include "Scene/Object/PickUp/PickUp.h"
#include "Team/DWTeamModule.h"
#include "Vitality/DWVitality.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Voxel.h"
#include "Voxel/Voxels/Auxiliary/VoxelAuxiliary.h"

// Sets default values
ADWVoxelChunk::ADWVoxelChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Characters = TArray<ADWCharacter*>();
	Vitalitys = TArray<ADWVitality*>();
}

// Called when the game starts or when spawned
void ADWVoxelChunk::BeginPlay()
{
	Super::BeginPlay();

}

void ADWVoxelChunk::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnCollision(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ADWVoxelChunk::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ADWVoxelChunk::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// Called every frame
void ADWVoxelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADWVoxelChunk::OnSpawn_Implementation()
{
	Super::OnSpawn_Implementation();
}

void ADWVoxelChunk::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();
	
	AVoxelModule::GetWorldData<FDWWorldSaveData>()->SetChunkData(Index, *static_cast<FDWChunkSaveData*>(ToData()));
	
	Characters.Empty();
	Vitalitys.Empty();
}

void ADWVoxelChunk::Initialize(FIndex InIndex, int32 InBatch)
{
	Super::Initialize(InIndex, InBatch);
}

void ADWVoxelChunk::LoadData(FSaveData* InSaveData)
{
	FDWChunkSaveData ChunkData = *static_cast<FDWChunkSaveData*>(InSaveData);
	for (int32 i = 0; i < ChunkData.VoxelItems.Num(); i++)
	{
		const FVoxelItem& voxelItem = ChunkData.VoxelItems[i];
		SetVoxelComplex(voxelItem.Index, voxelItem);
	}
}

FSaveData* ADWVoxelChunk::ToData(bool bSaved)
{
	static FDWChunkSaveData ChunkData;

	ChunkData.Index = Index;
	ChunkData.bSaved = bSaved;

	for (auto& iter : VoxelMap)
	{
		ChunkData.VoxelItems.Add(iter.Value);
	}

	for (int32 i = 0; i < PickUps.Num(); i++)
	{
		ChunkData.PickUpDatas.Add(*static_cast<FPickUpSaveData*>(PickUps[i]->ToData()));
	}
	
	for (int32 i = 0; i < Characters.Num(); i++)
	{
		if(Characters[i]->GetNature() != ECharacterNature::Player)
		{
			ChunkData.CharacterDatas.Add(*static_cast<FCharacterSaveData*>(Characters[i]->ToData()));
		}
	}
	
	for (int32 i = 0; i < Vitalitys.Num(); i++)
	{
		ChunkData.VitalityDatas.Add(*static_cast<FVitalitySaveData*>(Vitalitys[i]->ToData()));
	}

	return &ChunkData;
}

void ADWVoxelChunk::Generate(bool bPreview)
{
	if (VoxelMap.Num() > 0)
	{
		if(bGenerated)
		{
			GenerateMap();
		}
		if (SolidMesh)
		{
			if(!SolidMesh->IsEmpty())
			{
				SolidMesh->CreateMesh();
			}
			else
			{
				DestroySolidMesh();
			}
		}
		if (SemiMesh)
		{
			if(!SemiMesh->IsEmpty())
			{
				SemiMesh->CreateMesh();
			}
			else
			{
				DestroySemiMesh();
			}
		}
		if (TransMesh)
		{
			if(!TransMesh->IsEmpty())
			{
				TransMesh->CreateMesh();
			}
			else
			{
				DestroyTransMesh();
			}
		}
	}
	else
	{
		DestroySolidMesh();
		DestroySemiMesh();
		DestroyTransMesh();
	}

	if(!bGenerated)
	{
		OnGenerated(bPreview);
	}
}

void ADWVoxelChunk::BuildMap()
{
	Super::BuildMap();
}

void ADWVoxelChunk::GenerateMap()
{
	Super::GenerateMap();
}

void ADWVoxelChunk::SpawnActors()
{
	Super::SpawnActors();
	if (AVoxelModule::GetWorldData<FDWWorldSaveData>()->IsExistChunkData(Index))
	{
		FDWChunkSaveData ChunkData = AVoxelModule::GetWorldData<FDWWorldSaveData>()->GetChunkData(Index);
		for (int32 i = 0; i < ChunkData.PickUpDatas.Num(); i++)
		{
			SpawnPickUp(ChunkData.PickUpDatas[i]);
		}
		for (int32 i = 0; i < ChunkData.CharacterDatas.Num(); i++)
		{
			SpawnCharacter(ChunkData.CharacterDatas[i]);
		}
		for (int32 i = 0; i < ChunkData.VitalityDatas.Num(); i++)
		{
			SpawnVitality(ChunkData.VitalityDatas[i]);
		}
	}
	else if(SolidMesh || SemiMesh)
	{
		if (FIndex::Distance(Index, AVoxelModule::GetWorldData<FDWWorldSaveData>()->LastVitalityRaceIndex) > 250.f / AVoxelModule::GetWorldData()->VitalityRaceDensity)
		{
			auto raceData = UDWHelper::RandomVitalityRaceData();
			for (int32 i = 0; i < raceData.Items.Num(); i++)
			{
				auto vitalityItem = raceData.Items[i];
				auto vitalityData = vitalityItem.GetData<UVitalityAssetBase>();
				for (int32 j = 0; j < vitalityItem.Count; j++)
				{
					for (int32 k = 0; k < 10; k++)
					{
						FHitResult hitResult;
						if (AVoxelModule::Get()->ChunkTraceSingle(this, FMath::Max(vitalityData->Range.X, vitalityData->Range.Y) * 0.5f * AVoxelModule::GetWorldData()->BlockSize, vitalityData->Range.Z * 0.5f * AVoxelModule::GetWorldData()->BlockSize, hitResult))
						{
							auto saveData = FVitalitySaveData();
							saveData.ID = vitalityData->GetPrimaryAssetId();
							saveData.Name = vitalityData->Name.ToString();
							saveData.RaceID = raceData.ID.ToString();
							saveData.Level = vitalityItem.Level;
							saveData.SpawnLocation = hitResult.Location;
							saveData.SpawnRotation = FRotator(0, FMath::RandRange(0, 360), 0);
							SpawnVitality(saveData);
							AVoxelModule::GetWorldData<FDWWorldSaveData>()->LastVitalityRaceIndex = Index;
							break;
						}
					}
				}
			}
		}
		if (FIndex::Distance(Index, AVoxelModule::GetWorldData<FDWWorldSaveData>()->LastCharacterRaceIndex) > 300.f / AVoxelModule::GetWorldData()->CharacterRaceDensity)
		{
			ADWCharacter* captain = nullptr;
			auto raceData = UDWHelper::RandomCharacterRaceData();
			for (int32 i = 0; i < raceData.Items.Num(); i++)
			{
				auto characterItem = raceData.Items[i];
				auto characterData = characterItem.GetData<UCharacterAssetBase>();
				for (int32 j = 0; j < characterItem.Count; j++)
				{
					for (int32 k = 0; k < 10; k++)
					{
						FHitResult hitResult;
						if (AVoxelModule::Get()->ChunkTraceSingle(this, FMath::Max(characterData->Range.X, characterData->Range.Y) * 0.5f * AVoxelModule::GetWorldData()->BlockSize, characterData->Range.Z * 0.5f * AVoxelModule::GetWorldData()->BlockSize, hitResult))
						{
							auto saveData = FCharacterSaveData();
							saveData.ID = characterData->GetPrimaryAssetId();
							saveData.Name = characterData->Name.ToString();
							saveData.RaceID = raceData.ID.ToString();
							saveData.Level = characterItem.Level;
							saveData.SpawnLocation = hitResult.Location;
							saveData.SpawnRotation = FRotator(0, FMath::RandRange(0, 360), 0);
							if(auto character = SpawnCharacter(saveData))
							{
								if (captain == nullptr)
								{
									captain = character;
									if(ADWTeamModule* TeamModule = AMainModule::GetModuleByClass<ADWTeamModule>())
									{
										TeamModule->CreateTeam(captain);
									}
								}
								else
								{
									captain->AddTeamMate(character);
								}
							}
							AVoxelModule::GetWorldData<FDWWorldSaveData>()->LastCharacterRaceIndex = Index;
							break;
						}
					}
				}
			}
		}
	}
}

void ADWVoxelChunk::DestroyActors()
{
	for (int32 i = 0; i < PickUps.Num(); i++)
	{
		PickUps[i]->Destroy();
	}
	for (int32 i = 0; i < Characters.Num(); i++)
	{
		Characters[i]->Destroy();
	}
	for (int32 i = 0; i < Vitalitys.Num(); i++)
	{
		Vitalitys[i]->Destroy();
	}
}

void ADWVoxelChunk::OnGenerated(bool bPreview)
{
	bGenerated = true;

	for (auto& iter : VoxelMap)
	{
		SpawnAuxiliary(iter.Value);
	}
	
	if(!bPreview)
	{
		SpawnActors();
	}
}

void ADWVoxelChunk::AddSceneObject(ISceneObjectInterface* InSceneObject)
{
	Super::AddSceneObject(InSceneObject);

	if(ADWCharacter* Character = Cast<ADWCharacter>(InSceneObject))
	{
		AttachCharacter(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InSceneObject))
	{
		AttachVitality(Vitality);
	}
}

void ADWVoxelChunk::RemoveSceneObject(ISceneObjectInterface* InSceneObject)
{
	Super::RemoveSceneObject(InSceneObject);

	if(ADWCharacter* Character = Cast<ADWCharacter>(InSceneObject))
	{
		DetachCharacter(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InSceneObject))
	{
		DetachVitality(Vitality);
	}
}

void ADWVoxelChunk::DestroySceneObject(ISceneObjectInterface* InSceneObject)
{
	Super::DestroySceneObject(InSceneObject);

	if(ADWCharacter* Character = Cast<ADWCharacter>(InSceneObject))
	{
		DestroyCharacter(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InSceneObject))
	{
		DestroyVitality(Vitality);
	}
}

ADWCharacter* ADWVoxelChunk::SpawnCharacter(FCharacterSaveData InSaveData)
{
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ADWCharacter* character = GetWorld()->SpawnActor<ADWCharacter>(InSaveData.GetCharacterData()->Class, spawnParams))
	{
		character->LoadData(&InSaveData);
		character->SpawnDefaultController();
		AttachCharacter(character);
		return character;
	}
	return nullptr;
}

void ADWVoxelChunk::AttachCharacter(ADWCharacter* InCharacter)
{
	if(!InCharacter || !InCharacter->IsValidLowLevel() || Characters.Contains(InCharacter)) return;

	// InCharacter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	InCharacter->SetOwnerChunk(this);
	Characters.Add(InCharacter);
}

void ADWVoxelChunk::DetachCharacter(ADWCharacter* InCharacter)
{
	if(!InCharacter || !InCharacter->IsValidLowLevel() || !Characters.Contains(InCharacter)) return;

	// InCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InCharacter->SetOwnerChunk(nullptr);
	Characters.Remove(InCharacter);
}

void ADWVoxelChunk::DestroyCharacter(ADWCharacter* InCharacter)
{
	if (!InCharacter || !InCharacter->IsValidLowLevel()) return;

	if (Characters.Contains(InCharacter))
	{
		Characters.Remove(InCharacter);
		InCharacter->Destroy();
	}
}

ADWVitality* ADWVoxelChunk::SpawnVitality(FVitalitySaveData InSaveData)
{
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ADWVitality* Vitality = GetWorld()->SpawnActor<ADWVitality>(InSaveData.GetVitalityData()->Class, spawnParams))
	{
		Vitality->LoadData(&InSaveData);
		AttachVitality(Vitality);
		return Vitality;
	}
	return nullptr;
}

void ADWVoxelChunk::AttachVitality(ADWVitality* InVitality)
{
	if(!InVitality || !InVitality->IsValidLowLevel() || Vitalitys.Contains(InVitality)) return;

	InVitality->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	InVitality->SetOwnerChunk(this);
	Vitalitys.Add(InVitality);
}

void ADWVoxelChunk::DetachVitality(ADWVitality* InVitality)
{
	if(!InVitality || !InVitality->IsValidLowLevel() || !Vitalitys.Contains(InVitality)) return;

	InVitality->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InVitality->SetOwnerChunk(nullptr);
	Vitalitys.Remove(InVitality);
}

void ADWVoxelChunk::DestroyVitality(ADWVitality* InVitality)
{
	if (!InVitality || !InVitality->IsValidLowLevel()) return;

	if (Vitalitys.Contains(InVitality))
	{
		Vitalitys.Remove(InVitality);
		InVitality->Destroy();
	}
}
