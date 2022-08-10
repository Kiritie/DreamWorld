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
#include "Voxel/VoxelModuleBPLibrary.h"

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

void ADWVoxelChunk::OnSpawn_Implementation(const TArray<FParameter>& InParams)
{
	Super::OnSpawn_Implementation(InParams);
}

void ADWVoxelChunk::OnDespawn_Implementation()
{
	Super::OnDespawn_Implementation();
}

void ADWVoxelChunk::LoadData(FSaveData* InSaveData, bool bForceMode)
{
	Super::LoadData(InSaveData, bForceMode);
}
 
FSaveData* ADWVoxelChunk::ToData()
{
	static FDWVoxelChunkSaveData SaveData;
	SaveData = Super::ToData()->CastRef<FVoxelChunkSaveData>();

	for(auto& Iter : Characters)
	{
		if(Iter->GetNature() != EDWCharacterNature::Player)
		{
			SaveData.CharacterDatas.Add(Iter->ToSaveDataRef<FDWCharacterSaveData>(true));
		}
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
	// 			const FVector point = characterPart->GetComponentLocation() + characterPart->GetScaledCapsuleRadius() * normal + UVoxelModuleBPLibrary::GetWorldData().GetBlockSizedNormal(normal);
	// 			if(FVector::Distance(character->GetActorLocation(), point) > 100)
	// 			{
	// 				Debug(FString::Printf(TEXT("%f"), FVector::Distance(character->GetActorLocation(), point)));
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
	// 			const FVector point = characterPart->GetComponentLocation() - characterPart->GetScaledCapsuleRadius() * normal - UVoxelModuleBPLibrary::GetWorldData().GetBlockSizedNormal(normal);
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

// Called every frame
void ADWVoxelChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADWVoxelChunk::Initialize(AVoxelModule* InModule, FIndex InIndex, int32 InBatch)
{
	Super::Initialize(InModule, InIndex, InBatch);
}

void ADWVoxelChunk::Generate(bool bForceMode)
{
	Super::Generate(bForceMode);
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

	if(UVoxelModuleBPLibrary::GetWorldData().IsExistChunkData(Index))
	{
		auto chunkData = *UVoxelModuleBPLibrary::GetWorldData().GetChunkData<FDWVoxelChunkSaveData>(Index);
		for(int32 i = 0; i < chunkData.VitalityDatas.Num(); i++)
		{
			UAbilityModuleBPLibrary::SpawnVitality(&chunkData.VitalityDatas[i], this);
		}
		for(int32 i = 0; i < chunkData.CharacterDatas.Num(); i++)
		{
			UAbilityModuleBPLibrary::SpawnCharacter(&chunkData.CharacterDatas[i], this);
		}
	}
	else if(IsEntity())
	{
		if(FIndex::Distance(Index, UVoxelModuleBPLibrary::GetWorldData().LastVitalityRaceIndex) > UVoxelModuleBPLibrary::GetWorldData().VitalityRaceDensity / UVoxelModuleBPLibrary::GetWorldData().GetChunkLength())
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
						if(Module->ChunkTraceSingle(this, FMath::Max(vitalityData.Range.X, vitalityData.Range.Y) * 0.5f * UVoxelModuleBPLibrary::GetWorldData().BlockSize, vitalityData.Range.Z * 0.5f * UVoxelModuleBPLibrary::GetWorldData().BlockSize, hitResult))
						{
							auto saveData = FDWVitalitySaveData();
							saveData.ID = vitalityData.GetPrimaryAssetId();
							saveData.Name = *vitalityData.Name.ToString();
							saveData.RaceID = raceData.ID;
							saveData.Level = vitalityItem.Level;
							saveData.SpawnLocation = hitResult.Location;
							saveData.SpawnRotation = FRotator(0, FMath::RandRange(0, 360), 0);
							UAbilityModuleBPLibrary::SpawnVitality(&saveData, this);
							UVoxelModuleBPLibrary::GetWorldData().LastVitalityRaceIndex = Index;
							break;
						}
					}
				}
			}
		}
		if(FIndex::Distance(Index, UVoxelModuleBPLibrary::GetWorldData().LastCharacterRaceIndex) > UVoxelModuleBPLibrary::GetWorldData().CharacterRaceDensity / UVoxelModuleBPLibrary::GetWorldData().GetChunkLength())
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
						if(Module->ChunkTraceSingle(this, FMath::Max(characterData.Range.X, characterData.Range.Y) * 0.5f * UVoxelModuleBPLibrary::GetWorldData().BlockSize, characterData.Range.Z * 0.5f * UVoxelModuleBPLibrary::GetWorldData().BlockSize, hitResult))
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
							UVoxelModuleBPLibrary::GetWorldData().LastCharacterRaceIndex = Index;
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

	for(auto Iter : Characters)
	{
		UObjectPoolModuleBPLibrary::DespawnObject(Iter);
	}
	Characters.Empty();

	for(auto Iter : Vitalitys)
	{
		UObjectPoolModuleBPLibrary::DespawnObject(Iter);
	}
	Vitalitys.Empty();
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