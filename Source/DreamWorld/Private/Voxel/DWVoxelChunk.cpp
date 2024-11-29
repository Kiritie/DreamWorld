// Fill out your copyright notice in the Description page of Project Settings.PickUp/


#include "Voxel/DWVoxelChunk.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Character/DWCharacter.h"
#include "Character/DWCharacterData.h"
#include "Common/CommonTypes.h"
#include "Team/DWTeamModule.h"
#include "Vitality/DWVitality.h"
#include "Voxel/VoxelModule.h"
#include "SaveGame/SaveGameModuleStatics.h"
#include "Vitality/DWVitalityData.h"
#include "Voxel/VoxelModuleStatics.h"

// Sets default values
ADWVoxelChunk::ADWVoxelChunk()
{
}

void ADWVoxelChunk::OnSpawn_Implementation(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InOwner, InParams);
}

void ADWVoxelChunk::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
}

void ADWVoxelChunk::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}

FSaveData* ADWVoxelChunk::ToData()
{
	static FDWVoxelChunkSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FVoxelChunkSaveData>();

	for(auto& Iter : SceneActorMap)
	{
		if(ADWCharacter* Character = Cast<ADWCharacter>(Iter.Value))
		{
			if(!Character->IsPlayer() && !Character->IsDead())
			{
				SaveData.CharacterDatas.Add(Character->GetSaveDataRef<FDWCharacterSaveData>(true));
			}
		}
		if(ADWVitality* Vitality = Cast<ADWVitality>(Iter.Value))
		{
			if(!Vitality->IsDead())
			{
				SaveData.VitalityDatas.Add(Vitality->GetSaveDataRef<FDWVitalitySaveData>(true));
			}
		}
	}

	return &SaveData;
}

void ADWVoxelChunk::Initialize(FIndex InIndex, int32 InBatch)
{
	Super::Initialize(InIndex, InBatch);
}

void ADWVoxelChunk::Generate(EPhase InPhase)
{
	Super::Generate(InPhase);
}

void ADWVoxelChunk::BuildMap(int32 InStage)
{
	Super::BuildMap(InStage);
}

void ADWVoxelChunk::BuildMesh()
{
	Super::BuildMesh();
}

void ADWVoxelChunk::LoadSceneActors(FSaveData* InSaveData)
{
	Super::LoadSceneActors(InSaveData);

	auto& SaveData = InSaveData->CastRef<FDWVoxelChunkSaveData>();
	for(auto& Iter : SaveData.VitalityDatas)
	{
		UAbilityModuleStatics::SpawnAbilityActor(&Iter, this);
	}
	for(auto& Iter : SaveData.CharacterDatas)
	{
		UAbilityModuleStatics::SpawnAbilityActor(&Iter, this);
	}
}

void ADWVoxelChunk::SpawnSceneActors()
{
	Super::SpawnSceneActors();

	const auto& WorldData = UVoxelModule::Get().GetWorldData();
	const FVector2D WorldLocation = FVector2D(GetChunkLocation().X + WorldData.GetChunkRealSize().X * 0.5f, GetChunkLocation().Y + WorldData.GetChunkRealSize().Y * 0.5f) / WorldData.BlockSize;

	TArray<FVitalityRaceData> VitalityRaceDatas;
	if(UAbilityModuleStatics::GetNoiseRaceDatas(WorldLocation, WorldData.WorldSeed, VitalityRaceDatas))
	{
		for(auto& RaceData : VitalityRaceDatas)
		{
			for(auto& VitalityItem : RaceData.Items)
			{
				const auto& VitalityData = VitalityItem.GetData<UDWVitalityData>();
				const int32 Num = WorldData.RandomStream.RandRange(VitalityItem.MinCount, VitalityItem.MaxCount);
				const int32 Level = FMath::Clamp(WorldData.RandomStream.RandRange(RaceData.MinLevel, RaceData.MaxLevel), 1, VitalityData.MaxLevel);
				DON(Num,
					FHitResult HitResult;
					if(UVoxelModuleStatics::VoxelAgentTraceSingle(Index, VitalityData.Radius, VitalityData.HalfHeight, {}, HitResult, true, 10, false, false))
					{
						auto SaveData = FDWVitalitySaveData();
						SaveData.AssetID = VitalityData.GetPrimaryAssetId();
						SaveData.Name = *VitalityData.Name.ToString();
						SaveData.RaceID = RaceData.ID;
						SaveData.Level = Level;
						SaveData.SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f), HitResult.Location, FVector::OneVector);
						SaveData.InitInventoryData(WorldData.RandomStream);
						UAbilityModuleStatics::SpawnAbilityActor(&SaveData, UVoxelModuleStatics::FindChunkByLocation(SaveData.SpawnTransform.GetLocation()));
					}
				)
			}
		}
	}
	TArray<FCharacterRaceData> CharacterRaceDatas;
	if(UAbilityModuleStatics::GetNoiseRaceDatas(WorldLocation, WorldData.WorldSeed, CharacterRaceDatas))
	{
		for(auto& RaceData : CharacterRaceDatas)
		{
			ADWCharacter* Captain = nullptr;
			for(auto& CharacterItem : RaceData.Items)
			{
				const auto& CharacterData = CharacterItem.GetData<UDWCharacterData>();
				const int32 Num = WorldData.RandomStream.RandRange(CharacterItem.MinCount, CharacterItem.MaxCount);
				const int32 Level = FMath::Clamp(WorldData.RandomStream.RandRange(RaceData.MinLevel, RaceData.MaxLevel), 1, CharacterData.MaxLevel);
				DON(Num,
					FHitResult HitResult;
					if(UVoxelModuleStatics::VoxelAgentTraceSingle(Index, CharacterData.Radius, CharacterData.HalfHeight, {}, HitResult, true, 10, false, true))
					{
						auto SaveData = FDWCharacterSaveData();
						SaveData.AssetID = CharacterData.GetPrimaryAssetId();
						SaveData.Name = *CharacterData.Name.ToString();
						SaveData.RaceID = RaceData.ID;
						SaveData.Level = Level;
						SaveData.SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f), HitResult.Location, FVector::OneVector);
						SaveData.InitInventoryData(WorldData.RandomStream);
						if(CharacterData.Dialogues.Num() > 0)
						{
							SaveData.Dialogue = CharacterData.Dialogues[WorldData.RandomStream.RandRange(0, CharacterData.Dialogues.Num() - 1)];
						}
						if(ADWCharacter* Character = Cast<ADWCharacter>(UAbilityModuleStatics::SpawnAbilityActor(&SaveData, UVoxelModuleStatics::FindChunkByLocation(SaveData.SpawnTransform.GetLocation()))))
						{
							if(!Captain)
							{
								Captain = Character;
								UDWTeamModule::Get().CreateTeam(Captain);
							}
							else
							{
								Captain->AddTeamMate(Character);
							}
						}
					}
				)
			}
		}
	}
}
