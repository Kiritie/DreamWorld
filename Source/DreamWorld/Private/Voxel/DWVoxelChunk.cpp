// Fill out your copyright notice in the Description page of Project Settings.PickUp/


#include "Voxel/DWVoxelChunk.h"

#include "Ability/Character/CharacterDataBase.h"
#include "Ability/Vitality/VitalityDataBase.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Character/Base/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Main/MainModule.h"
#include "Scene/Actor/PickUp/PickUp.h"
#include "Team/DWTeamModule.h"
#include "Vitality/DWVitality.h"
#include "Vitality/DWVitalityData.h"
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
	FDWVoxelChunkSaveData ChunkData = *static_cast<FDWVoxelChunkSaveData*>(InSaveData);
	for(int32 i = 0; i < ChunkData.VoxelItems.Num(); i++)
	{
		const FVoxelItem& voxelItem = ChunkData.VoxelItems[i];
		SetVoxelComplex(voxelItem.Index, voxelItem);
	}
}
 
FSaveData* ADWVoxelChunk::ToData(bool bSaved)
{
	static FDWVoxelChunkSaveData ChunkData;

	ChunkData.Index = Index;
	ChunkData.bSaved = bSaved;

	for(auto& iter : VoxelMap)
	{
		ChunkData.VoxelItems.Add(iter.Value);
	}

	for(int32 i = 0; i < PickUps.Num(); i++)
	{
		ChunkData.PickUpDatas.Add(*static_cast<FPickUpSaveData*>(PickUps[i]->ToData()));
	}

	for(int32 i = 0; i < Characters.Num(); i++)
	{
		if(Characters[i]->GetNature() != EDWCharacterNature::Player)
		{
			ChunkData.CharacterDatas.Add(*static_cast<FDWCharacterSaveData*>(Characters[i]->ToData()));
		}
	}

	for(int32 i = 0; i < Vitalitys.Num(); i++)
	{
		ChunkData.VitalityDatas.Add(*static_cast<FDWVitalitySaveData*>(Vitalitys[i]->ToData()));
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
			SpawnPickUp(ChunkData.PickUpDatas[i]);
		}
		for(int32 i = 0; i < ChunkData.CharacterDatas.Num(); i++)
		{
			SpawnCharacter(ChunkData.CharacterDatas[i]);
		}
		for(int32 i = 0; i < ChunkData.VitalityDatas.Num(); i++)
		{
			SpawnVitality(ChunkData.VitalityDatas[i]);
		}
	}
	else if(SolidMesh || SemiMesh)
	{
		if(FIndex::Distance(Index, AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->LastVitalityRaceIndex) > 250.f / AVoxelModule::GetWorldData()->VitalityRaceDensity)
		{
			auto raceData = UDWHelper::RandomVitalityRaceData();
			for(int32 i = 0; i < raceData.Items.Num(); i++)
			{
				const FAbilityItem& vitalityItem = raceData.Items[i];
				const UVitalityDataBase& vitalityData = vitalityItem.GetData<UVitalityDataBase>();
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
							saveData.Initialize();
							SpawnVitality(saveData);
							AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->LastVitalityRaceIndex = Index;
							break;
						}
					}
				}
			}
		}
		if(FIndex::Distance(Index, AVoxelModule::GetWorldData<FDWVoxelWorldSaveData>()->LastCharacterRaceIndex) > 300.f / AVoxelModule::GetWorldData()->CharacterRaceDensity)
		{
			ADWCharacter* captain = nullptr;
			auto raceData = UDWHelper::RandomCharacterRaceData();
			for(int32 i = 0; i < raceData.Items.Num(); i++)
			{
				const FAbilityItem& characterItem = raceData.Items[i];
				const UCharacterDataBase& characterData = characterItem.GetData<UCharacterDataBase>();
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
							saveData.Initialize();
							if(auto character = SpawnCharacter(saveData))
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

	for(int32 i = 0; i < PickUps.Num(); i++)
	{
		PickUps[i]->Destroy();
	}
	PickUps.Empty();

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

	if(ADWCharacter* Character = Cast<ADWCharacter>(InActor))
	{
		AttachCharacter(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InActor))
	{
		AttachVitality(Vitality);
	}
}

void ADWVoxelChunk::RemoveSceneActor(AActor* InActor)
{
	Super::RemoveSceneActor(InActor);

	if(ADWCharacter* Character = Cast<ADWCharacter>(InActor))
	{
		DetachCharacter(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InActor))
	{
		DetachVitality(Vitality);
	}
}

void ADWVoxelChunk::DestroySceneActor(AActor* InActor)
{
	Super::DestroySceneActor(InActor);

	if(ADWCharacter* Character = Cast<ADWCharacter>(InActor))
	{
		DestroyCharacter(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InActor))
	{
		DestroyVitality(Vitality);
	}
}

ADWCharacter* ADWVoxelChunk::SpawnCharacter(FDWCharacterSaveData InSaveData)
{
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if(ADWCharacter* character = GetWorld()->SpawnActor<ADWCharacter>(InSaveData.GetCharacterData().Class, spawnParams))
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
	if(!InCharacter || !InCharacter->IsValidLowLevel()) return;

	if(Characters.Contains(InCharacter))
	{
		Characters.Remove(InCharacter);
		InCharacter->Destroy();
	}
}

ADWVitality* ADWVoxelChunk::SpawnVitality(FDWVitalitySaveData InSaveData)
{
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if(ADWVitality* Vitality = GetWorld()->SpawnActor<ADWVitality>(InSaveData.GetVitalityData().Class, spawnParams))
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
	if(!InVitality || !InVitality->IsValidLowLevel()) return;

	if(Vitalitys.Contains(InVitality))
	{
		Vitalitys.Remove(InVitality);
		InVitality->Destroy();
	}
}
