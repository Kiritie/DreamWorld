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
#include "Global/GlobalTypes.h"
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
#include "Voxel/VoxelModuleBPLibrary.h"

// Sets default values
ADWVoxelChunk::ADWVoxelChunk()
{
	Characters = TArray<ADWCharacter*>();
	Vitalitys = TArray<ADWVitality*>();

	bRaceGenerated = false;
}

void ADWVoxelChunk::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWVoxelChunk::OnDespawn_Implementation(bool bRecovery)
{
	Super::OnDespawn_Implementation(bRecovery);
	
	bRaceGenerated = false;
}

void ADWVoxelChunk::LoadData(FSaveData* InSaveData, EPhase InPhase)
{
	Super::LoadData(InSaveData, InPhase);
}
 
FSaveData* ADWVoxelChunk::ToData()
{
	static FDWVoxelChunkSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FVoxelChunkSaveData>();

	for(auto& Iter : Characters)
	{
		SaveData.CharacterDatas.Add(Iter->ToSaveDataRef<FDWCharacterSaveData>(true));
	}

	for(auto& Iter : Vitalitys)
	{
		SaveData.VitalityDatas.Add(Iter->ToSaveDataRef<FDWVitalitySaveData>(true));
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
	// 			const FVector normal = FVector(FMath::Min((character->GetMoveDirection(false) * 2.5f).X, 1.f), FMath::Min((character->GetMoveDirection(false) * 2.5f).Y, 1.f), FMath::Min((character->GetMoveDirection(false) * 2.5f).Z, 1.f));
	// 			const FVector point = characterPart->GetComponentLocation() + characterPart->GetScaledCapsuleRadius() * normal + AVoxelModule::Get()->GetWorldData().GetBlockSizedNormal(normal);
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
	// 			const FVector normal = FVector(FMath::Min((character->GetMoveDirection(false) * 2.5f).X, 1.f), FMath::Min((character->GetMoveDirection(false) * 2.5f).Y, 1.f), FMath::Min((character->GetMoveDirection(false) * 2.5f).Z, 1.f));
	// 			const FVector point = characterPart->GetComponentLocation() - characterPart->GetScaledCapsuleRadius() * normal - AVoxelModule::Get()->GetWorldData().GetBlockSizedNormal(normal);
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

void ADWVoxelChunk::SpawnActors()
{
	Super::SpawnActors();
}

void ADWVoxelChunk::LoadActors(FSaveData* InSaveData)
{
	Super::LoadActors(InSaveData);

	auto& SaveData = InSaveData->CastRef<FDWVoxelChunkSaveData>();
	for(int32 i = 0; i < SaveData.VitalityDatas.Num(); i++)
	{
		UAbilityModuleBPLibrary::SpawnVitality(&SaveData.VitalityDatas[i], this);
	}
	for(int32 i = 0; i < SaveData.CharacterDatas.Num(); i++)
	{
		UAbilityModuleBPLibrary::SpawnCharacter(&SaveData.CharacterDatas[i], this);
	}
}

void ADWVoxelChunk::GenerateActors()
{
	Super::GenerateActors();

	const auto& worldData = AVoxelModule::Get()->GetWorldData();
	const FVector2D worldLocation = FVector2D(GetChunkLocation().X, GetChunkLocation().Y) + worldData.GetChunkLength() * 0.5f;
	
	TArray<FVitalityRaceData> vitalityRaceDatas;
	if(UAbilityModuleBPLibrary::GetNoiseRaceDatas(worldLocation, worldData.WorldSeed, vitalityRaceDatas))
	{
		for(auto& raceData : vitalityRaceDatas)
		{
			for(auto& vitalityItem : raceData.Items)
			{
				const auto& vitalityData = vitalityItem.GetData<UAbilityVitalityDataBase>();
				const int32 tmpNum = vitalityItem.Count != 0 ? vitalityItem.Count : worldData.RandomStream.RandRange(vitalityItem.MinCount, vitalityItem.MaxCount);
				DON(i, tmpNum,
					FHitResult hitResult;
					if(UVoxelModuleBPLibrary::VoxelAgentTraceSingle(Index, vitalityData.Radius, vitalityData.HalfHeight, {}, hitResult, true, 10, false))
					{
						auto saveData = FDWVitalitySaveData();
						saveData.ID = vitalityData.GetPrimaryAssetId();
						saveData.Name = *vitalityData.Name.ToString();
						saveData.RaceID = raceData.ID;
						saveData.Level = vitalityItem.Level;
						saveData.SpawnLocation = hitResult.Location;
						saveData.SpawnRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);
						saveData.InventoryData = vitalityData.InventoryData;
						UAbilityModuleBPLibrary::SpawnVitality(&saveData, this);
					}
				)
			}
		}
	}
	TArray<FCharacterRaceData> characterRaceDatas;
	if(UAbilityModuleBPLibrary::GetNoiseRaceDatas(worldLocation, worldData.WorldSeed, characterRaceDatas))
	{
		for(auto& raceData : characterRaceDatas)
		{
			ADWCharacter* captain = nullptr;
			for(auto& characterItem : raceData.Items)
			{
				const auto& characterData = characterItem.GetData<UAbilityCharacterDataBase>();
				const int32 tmpNum = characterItem.Count != 0 ? characterItem.Count : worldData.RandomStream.RandRange(characterItem.MinCount, characterItem.MaxCount);
				DON(i, tmpNum,
					FHitResult hitResult;
					if(UVoxelModuleBPLibrary::VoxelAgentTraceSingle(Index, characterData.Radius, characterData.HalfHeight, {}, hitResult, true, 10, false))
					{
						auto saveData = FDWCharacterSaveData();
						saveData.ID = characterData.GetPrimaryAssetId();
						saveData.Name = *characterData.Name.ToString();
						saveData.RaceID = raceData.ID;
						saveData.Level = characterItem.Level;
						saveData.SpawnLocation = hitResult.Location;
						saveData.SpawnRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);
						saveData.InventoryData = characterData.InventoryData;
						if(ADWCharacter* character = Cast<ADWCharacter>(UAbilityModuleBPLibrary::SpawnCharacter(&saveData, this)))
						{
							if(!captain)
							{
								captain = character;
								ADWTeamModule::Get()->CreateTeam(captain);
							}
							else
							{
								captain->AddTeamMate(character);
							}
						}
					}
				)
			}
		}
	}
}

void ADWVoxelChunk::DestroyActors()
{
	Super::DestroyActors();

	UObjectPoolModuleBPLibrary::DespawnObjects(Characters);
	Characters.Empty();

	UObjectPoolModuleBPLibrary::DespawnObjects(Vitalitys);
	Vitalitys.Empty();
}

void ADWVoxelChunk::AddSceneActor(AActor* InActor)
{
	if(!InActor || !InActor->Implements<USceneActorInterface>() || ISceneActorInterface::Execute_GetContainer(InActor) == this) return;

	if(State != EVoxelChunkState::None)
	{
		if(ADWCharacter* Character = Cast<ADWCharacter>(InActor))
		{
			if(!Character->IsPlayer())
			{
				Characters.Add(Character);
			}
		}
		else if(ADWVitality* Vitality = Cast<ADWVitality>(InActor))
		{
			Vitalitys.Add(Vitality);
		}
	}

	Super::AddSceneActor(InActor);
}

void ADWVoxelChunk::RemoveSceneActor(AActor* InActor)
{
	if(!InActor || !InActor->Implements<USceneActorInterface>() || ISceneActorInterface::Execute_GetContainer(InActor) != this) return;

	if(ADWCharacter* Character = Cast<ADWCharacter>(InActor))
	{
		Characters.Remove(Character);
	}
	else if(ADWVitality* Vitality = Cast<ADWVitality>(InActor))
	{
		Vitalitys.Remove(Vitality);
	}

	Super::RemoveSceneActor(InActor);
}
