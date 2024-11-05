// Fill out your copyright notice in the Description page of Project Settings.PickUp/


#include "Voxel/DWVoxelChunk.h"

#include "Ability/AbilityModuleStatics.h"
#include "Ability/Character/AbilityCharacterDataBase.h"
#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Ability/Vitality/AbilityVitalityDataBase.h"
#include "Character/DWCharacter.h"
#include "Common/CommonTypes.h"
#include "Team/DWTeamModule.h"
#include "Vitality/DWVitality.h"
#include "Voxel/VoxelModule.h"
#include "ObjectPool/ObjectPoolModuleStatics.h"
#include "SaveGame/SaveGameModuleStatics.h"
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

void ADWVoxelChunk::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnCollision(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ADWVoxelChunk::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// if(OtherComp && OtherComp->IsA<UAbilityCharacterPart>())
	// {
	// 	if(UAbilityCharacterPart* characterPart = Cast<UAbilityCharacterPart>(OtherComp))
	// 	{
	// 		if(ACharacterBase* character = characterPart->GetOwnerCharacter())
	// 		{
	// 			Debug(FString::Printf(TEXT("%s"), *SweepResult.ImpactPoint.ToString()));
	// 			Debug(FString::Printf(TEXT("%s"), *SweepResult.ImpactNormal.ToString()));
	// 			const FVector normal = FVector(FMath::Min((character->GetMoveDirection() * 2.5f).X, 1.f), FMath::Min((character->GetMoveDirection() * 2.5f).Y, 1.f), FMath::Min((character->GetMoveDirection() * 2.5f).Z, 1.f));
	// 			const FVector point = characterPart->GetComponentLocation() + characterPart->GetScaledCapsuleRadius() * normal + UVoxelModule::Get().GetWorldData().GetBlockSizedNormal(normal);
	// 			if(FVector::Distance(character->GetLocation(), point) > 100)
	// 			{
	// 				Debug(FString::Printf(TEXT("%f"), FVector::Distance(character->GetLocation(), point)));
	// 			}
	// 			UKismetSystemLibrary::DrawDebugBox(this, point, FVector(5), FColor::Green, FRotator::ZeroRotator, 100);
	// 			const FVoxelItem& voxelItem = GetVoxelItem(LocationToIndex(point));
	// 			const UVoxelAsset VoxelData = voxelItem.GetData<UVoxelAsset>();
	// 			if(voxelItem.IsValid() && VoxelData.VoxelType != characterPart->GetLastOverlapVoxel())
	// 			{
	// 				if(UVoxel& voxel = voxelItem.GetVoxel())
	// 				{
	// 					characterPart->SetLastVoxelType(VoxelData.VoxelType);
	// 					voxel.OnTargetEnter(characterPart, FVoxelHitResult(voxelItem, point, normal));
	// 				}
	// 			}
	// 			character->OnBeginOverlapVoxel(characterPart, FVoxelHitResult(voxelItem, point, normal));
	// 		}
	// 	}
	// }
}

void ADWVoxelChunk::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// if(OtherComp && OtherComp->IsA<UAbilityCharacterPart>())
	// {
	// 	if(UAbilityCharacterPart* characterPart = Cast<UAbilityCharacterPart>(OtherComp))
	// 	{
	// 		if(ACharacterBase* character = characterPart->GetOwnerCharacter())
	// 		{
	// 			const FVector normal = FVector(FMath::Min((character->GetMoveDirection() * 2.5f).X, 1.f), FMath::Min((character->GetMoveDirection() * 2.5f).Y, 1.f), FMath::Min((character->GetMoveDirection() * 2.5f).Z, 1.f));
	// 			const FVector point = characterPart->GetComponentLocation() - characterPart->GetScaledCapsuleRadius() * normal - UVoxelModule::Get().GetWorldData().GetBlockSizedNormal(normal);
	// 			UKismetSystemLibrary::DrawDebugBox(this, point, FVector(5), FColor::Red, FRotator::ZeroRotator, 100);
	// 			const FVoxelItem& voxelItem = GetVoxelItem(LocationToIndex(point));
	// 			const UVoxelAsset VoxelData = voxelItem.GetData<UVoxelAsset>();
	// 			if(voxelItem.IsValid() && VoxelData.VoxelType == characterPart->GetLastOverlapVoxel())
	// 			{
	// 				const FVoxelItem& tmpVoxelItem = GetVoxelItem(LocationToIndex(characterPart->GetComponentLocation()));
	// 				const UVoxelAsset tmpVoxelData = tmpVoxelItem.GetData<UVoxelAsset>();
	// 				if(!tmpVoxelItem.IsValid() || tmpVoxelData.VoxelType != VoxelData.VoxelType)
	// 				{
	// 					if(UVoxel& voxel = voxelItem.GetVoxel())
	// 					{
	// 						characterPart->SetLastVoxelType(EVoxelType::Unknown);
	// 						voxel.OnTargetExit(characterPart, FVoxelHitResult(voxelItem, point, normal));
	// 					}
	// 				}
	// 			}
	// 			character->OnEndOverlapVoxel(characterPart, FVoxelHitResult(voxelItem, point, normal));
	// 		}
	// 	}
	// }
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
				const auto& VitalityData = VitalityItem.GetData<UAbilityVitalityDataBase>();
				const int32 tmpNum = VitalityItem.Count != 0 ? VitalityItem.Count : WorldData.RandomStream.RandRange(VitalityItem.MinCount, VitalityItem.MaxCount);
				DON(tmpNum,
					FHitResult HitResult;
					if(UVoxelModuleStatics::VoxelAgentTraceSingle(Index, VitalityData.Radius, VitalityData.HalfHeight, {}, HitResult, true, 10, false))
					{
						auto SaveData = FDWVitalitySaveData();
						SaveData.AssetID = VitalityData.GetPrimaryAssetId();
						SaveData.Name = *VitalityData.Name.ToString();
						SaveData.RaceID = RaceData.ID;
						SaveData.Level = VitalityItem.Level;
						SaveData.SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f), HitResult.Location, FVector::OneVector);
						SaveData.InitInventoryData();
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
				const auto& CharacterData = CharacterItem.GetData<UAbilityCharacterDataBase>();
				const int32 tmpNum = CharacterItem.Count != 0 ? CharacterItem.Count : WorldData.RandomStream.RandRange(CharacterItem.MinCount, CharacterItem.MaxCount);
				DON(tmpNum,
					FHitResult HitResult;
					if(UVoxelModuleStatics::VoxelAgentTraceSingle(Index, CharacterData.Radius, CharacterData.HalfHeight, {}, HitResult, true, 10, false))
					{
						auto SaveData = FDWCharacterSaveData();
						SaveData.AssetID = CharacterData.GetPrimaryAssetId();
						SaveData.Name = *CharacterData.Name.ToString();
						SaveData.RaceID = RaceData.ID;
						SaveData.Level = CharacterItem.Level;
						SaveData.SpawnTransform = FTransform(FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f), HitResult.Location, FVector::OneVector);
						SaveData.InitInventoryData();
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
