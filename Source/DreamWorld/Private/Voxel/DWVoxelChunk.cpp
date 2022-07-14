// Fill out your copyright notice in the Description page of Project Settings.PickUp/


#include "Voxel/DWVoxelChunk.h"

#include "Ability/AbilityModuleBPLibrary.h"
#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Ability/Vitality/AbilityVitalityDataBase.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/Base/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Main/MainModule.h"
#include "Team/DWTeamModule.h"
#include "Vitality/DWVitality.h"
#include "Vitality/DWVitalityData.h"
#include "Voxel/VoxelModule.h"
#include "Voxel/Components/VoxelMeshComponent.h"
#include "Voxel/Voxels/Voxel.h"
#include "Voxel/Voxels/Auxiliary/VoxelAuxiliary.h"
#include "ObjectPool/ObjectPoolModuleBPLibrary.h"
#include "SaveGame/SaveGameModuleBPLibrary.h"

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

void ADWVoxelChunk::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWVoxelChunk::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();
}

void ADWVoxelChunk::Initialize(FIndex InIndex, int32 InBatch)
{
	Super::Initialize(InIndex, InBatch);
}

void ADWVoxelChunk::LoadData(FSaveData* InSaveData)
{
	FDWVoxelChunkSaveData ChunkData = InSaveData->ToRef<FDWVoxelChunkSaveData>();
	for(int32 i = 0; i < ChunkData.VoxelItems.Num(); i++)
	{
		const FVoxelItem& voxelItem = ChunkData.VoxelItems[i];
		SetVoxelComplex(voxelItem.Index, voxelItem);
	}
}
 
FSaveData* ADWVoxelChunk::ToData()
{
	static FDWVoxelChunkSaveData ChunkData;

	ChunkData.Index = Index;

	for(auto& iter : VoxelMap)
	{
		ChunkData.VoxelItems.Add(iter.Value);
	}

	for(int32 i = 0; i < PickUps.Num(); i++)
	{
		ChunkData.PickUpDatas.Add(USaveGameModuleBPLibrary::ObjectToDataRef<FPickUpSaveData>(PickUps[i], true, false));
	}

	for(int32 i = 0; i < Characters.Num(); i++)
	{
		if(Characters[i]->GetNature() != EDWCharacterNature::Player)
		{
			ChunkData.CharacterDatas.Add(USaveGameModuleBPLibrary::ObjectToDataRef<FDWCharacterSaveData>(Characters[i], true, true));
		}
	}

	for(int32 i = 0; i < Vitalitys.Num(); i++)
	{
		ChunkData.VitalityDatas.Add(USaveGameModuleBPLibrary::ObjectToDataRef<FDWVitalitySaveData>(Vitalitys[i], true, true));
	}

	return &ChunkData;
}

void ADWVoxelChunk::Generate()
{
	Super::Generate();
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

	if(AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->IsExistChunkData(Index))
	{
		FDWVoxelChunkSaveData ChunkData = AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->GetChunkData(Index);
		for(int32 i = 0; i < ChunkData.PickUpDatas.Num(); i++)
		{
			UAbilityModuleBPLibrary::SpawnPickUp(&ChunkData.PickUpDatas[i], this);
		}
		for(int32 i = 0; i < ChunkData.VitalityDatas.Num(); i++)
		{
			UAbilityModuleBPLibrary::SpawnVitality(&ChunkData.VitalityDatas[i], this);
		}
		for(int32 i = 0; i < ChunkData.CharacterDatas.Num(); i++)
		{
			UAbilityModuleBPLibrary::SpawnCharacter(&ChunkData.CharacterDatas[i], this);
		}
	}
	else if(SolidMesh || SemiMesh)
	{
		if(FIndex::Distance(Index, AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->LastVitalityRaceIndex) > AVoxelModule::GetWorldData()->VitalityRaceDensity / AVoxelModule::GetWorldData()->GetChunkLength())
		{
			auto raceData = UAbilityModuleBPLibrary::RandomVitalityRaceData();
			for(int32 i = 0; i < raceData.Items.Num(); i++)
			{
				const FAbilityItem& vitalityItem = raceData.Items[i];
				const UAbilityVitalityDataBase& vitalityData = vitalityItem.GetData<UAbilityVitalityDataBase>();
				for(int32 j = 0; j < vitalityItem.Count; j++)
				{
					for(int32 k = 0; k < 10; k++)
					{
						FHitResult hitResult;
						if(AMainModule::GetModuleByClass<AVoxelModule>()->ChunkTraceSingle(this, FMath::Max(vitalityData.Range.X, vitalityData.Range.Y) * 0.5f * AVoxelModule::GetWorldData()->BlockSize, vitalityData.Range.Z * 0.5f * AVoxelModule::GetWorldData()->BlockSize, hitResult))
						{
							auto saveData = FDWVitalitySaveData();
							saveData.ID = vitalityData.GetPrimaryAssetId();
							saveData.Name = *vitalityData.Name.ToString();
							saveData.RaceID = raceData.ID;
							saveData.Level = vitalityItem.Level;
							saveData.SpawnLocation = hitResult.Location;
							saveData.SpawnRotation = FRotator(0, FMath::RandRange(0, 360), 0);
							UAbilityModuleBPLibrary::SpawnVitality(&saveData, this);
							AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->LastVitalityRaceIndex = Index;
							break;
						}
					}
				}
			}
		}
		if(FIndex::Distance(Index, AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->LastCharacterRaceIndex) > AVoxelModule::GetWorldData()->CharacterRaceDensity / AVoxelModule::GetWorldData()->GetChunkLength())
		{
			ADWCharacter* captain = nullptr;
			auto raceData = UAbilityModuleBPLibrary::RandomCharacterRaceData();
			for(int32 i = 0; i < raceData.Items.Num(); i++)
			{
				const FAbilityItem& characterItem = raceData.Items[i];
				const UAbilityCharacterDataBase& characterData = characterItem.GetData<UAbilityCharacterDataBase>();
				for(int32 j = 0; j < characterItem.Count; j++)
				{
					for(int32 k = 0; k < 10; k++)
					{
						FHitResult hitResult;
						if(AMainModule::GetModuleByClass<AVoxelModule>()->ChunkTraceSingle(this, FMath::Max(characterData.Range.X, characterData.Range.Y) * 0.5f * AVoxelModule::GetWorldData()->BlockSize, characterData.Range.Z * 0.5f * AVoxelModule::GetWorldData()->BlockSize, hitResult))
						{
							auto saveData = FDWCharacterSaveData();
							saveData.ID = characterData.GetPrimaryAssetId();
							saveData.Name = *characterData.Name.ToString();
							saveData.RaceID = raceData.ID;
							saveData.Level = characterItem.Level;
							saveData.SpawnLocation = hitResult.Location;
							saveData.SpawnRotation = FRotator(0, FMath::RandRange(0, 360), 0);
							if(ADWCharacter* character = Cast<ADWCharacter>(UAbilityModuleBPLibrary::SpawnCharacter(&saveData, this)))
							{
								if(captain == nullptr)
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
							AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->LastCharacterRaceIndex = Index;
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
	Super::DestroyActors();

	for(int32 i = 0; i < Characters.Num(); i++)
	{
		Characters[i]->Destroy();
	}
	Characters.Empty();

	for(int32 i = 0; i < Vitalitys.Num(); i++)
	{
		Vitalitys[i]->Destroy();
	}
	Vitalitys.Empty();
}

void ADWVoxelChunk::OnGenerated()
{
	Super::OnGenerated();
}

void ADWVoxelChunk::AddSceneActor(AActor* InActor)
{
	Super::AddSceneActor(InActor);

	if(!InActor || !InActor->Implements<USceneActorInterface>() || ISceneActorInterface::Execute_GetContainer(InActor) == this) return;

	if(ISceneActorInterface::Execute_GetContainer(InActor))
	{
		ISceneActorInterface::Execute_GetContainer(InActor)->RemoveSceneActor(InActor);
	}

	if(ADWCharacter* Character = Cast<ADWCharacter>(InActor))
	{
		// InCharacter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		Character->Execute_SetContainer(Character, this);
		Characters.Add(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InActor))
	{
		Vitality->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		Vitality->Execute_SetContainer(Vitality, this);
		Vitalitys.Add(Vitality);
	}
}

void ADWVoxelChunk::RemoveSceneActor(AActor* InActor)
{
	Super::RemoveSceneActor(InActor);

	if(!InActor || !InActor->Implements<USceneActorInterface>() || ISceneActorInterface::Execute_GetContainer(InActor) != this) return;

	if(ADWCharacter* Character = Cast<ADWCharacter>(InActor))
	{
		// InCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Character->Execute_SetContainer(Character, nullptr);
		Characters.Remove(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InActor))
	{
		Vitality->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Vitality->Execute_SetContainer(Vitality, nullptr);
		Vitalitys.Remove(Vitality);
	}
}