// Fill out your copyright notice in the Description page of Project Settings.PickUp/


#include "Voxel/DWVoxelChunk.h"

#include "Character/DWCharacter.h"
#include "Character/Base/CharacterBase.h"
#include "Components/BoxComponent.h"
#include "Scene/Object/PickUp/PickUp.h"
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
	
	AVoxelModule::GetWorldData().SetDWVoxelChunkData(Index, ToData());
	
	Characters.Empty();
	Vitalitys.Empty();
}

void ADWVoxelChunk::Initialize(FIndex InIndex, int32 InBatch)
{
	Super::Initialize(InIndex, InBatch);
}

void ADWVoxelChunk::LoadData(FSaveData* InDWVoxelChunkData)
{
	if(FDWChunkSaveData* ChunkSaveData = Cast<F>)
	for (int32 i = 0; i < InDWVoxelChunkData.VoxelItems.Num(); i++)
	{
		const FVoxelItem& voxelItem = InDWVoxelChunkData.VoxelItems[i];
		SetVoxelComplex(voxelItem.Index, voxelItem);
	}
}

FSaveData* ADWVoxelChunk::ToData(bool bSaved)
{
	FDWChunkSaveData DWVoxelChunkData;

	DWVoxelChunkData.Index = Index;
	DWVoxelChunkData.bSaved = bSaved;

	for (auto& iter : VoxelMap)
	{
		DWVoxelChunkData.VoxelItems.Add(iter.Value);
	}

	for (int32 i = 0; i < PickUps.Num(); i++)
	{
		DWVoxelChunkData.PickUpDatas.Add(PickUps[i]->ToData());
	}
	
	for (int32 i = 0; i < Characters.Num(); i++)
	{
		if(Characters[i]->GetNature() != ECharacterNature::Player)
		{
			DWVoxelChunkData.CharacterDatas.Add(*static_cast<FCharacterSaveData*>(Characters[i]->ToData()));
		}
	}
	
	for (int32 i = 0; i < Vitalitys.Num(); i++)
	{
		DWVoxelChunkData.VitalityDatas.Add(*static_cast<FVitalitySaveData*>(Vitalitys[i]->ToData()));
	}

	return DWVoxelChunkData;
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
	for (int32 x = 0; x < AVoxelModule::GetWorldData().ChunkSize; x++)
	{
		for (int32 y = 0; y < AVoxelModule::GetWorldData().ChunkSize; y++)
		{
			for (int32 z = 0; z < AVoxelModule::GetWorldData().ChunkSize; z++)
			{
				FIndex voxelIndex = FIndex(x, y, z);

				if (VoxelMap.Contains(voxelIndex)) continue;

				const EVoxelType voxelType = AVoxelModule::GetNoiseVoxelType(LocalIndexToWorld(voxelIndex));

				if (voxelType != EVoxelType::Empty)
				{
					switch (voxelType)
					{
						// grass
						case EVoxelType::Grass:
						{
							const float tmpNum = FMath::FRandRange(0.f, 1.f);
							// plant
							if (tmpNum < 0.2f)
							{
								SetVoxelComplex(FIndex(x, y, z + 1), FVoxelItem(FMath::FRandRange(0.f, 1.f) > 0.2f ? EVoxelType::Tall_Grass : (EVoxelType)FMath::RandRange((int32)EVoxelType::Flower_Allium, (int32)EVoxelType::Flower_Tulip_White)));
							}
							// tree
							else if (tmpNum < 0.21f)
							{
								if ((x > 2 && x <= AVoxelModule::GetWorldData().ChunkSize - 2) && (y > 2 && y <= AVoxelModule::GetWorldData().ChunkSize - 2))
								{
									const int32 treeHeight = FMath::RandRange(4.f, 5.f);
									const int32 leavesHeight = 2/*FMath::RandRange(2, 2)*/;
									const int32 leavesWidth = FMath::FRandRange(0.f, 1.f) < 0.5f ? 3 : 5;
									for (int32 trunkHeight = 0; trunkHeight < treeHeight; trunkHeight++)
									{
										SetVoxelComplex(FIndex(x, y, z + trunkHeight + 1), FVoxelItem(EVoxelType::Oak));
									}
									for (int32 offsetZ = treeHeight - leavesHeight; offsetZ < treeHeight + 1; offsetZ++)
									{
										for (int32 offsetX = -leavesWidth / 2; offsetX <= leavesWidth / 2; offsetX++)
										{
											for (int32 offsetY = -leavesWidth / 2; offsetY <= leavesWidth / 2; offsetY++)
											{
												const FVoxelItem& voxelItem = GetVoxelItem(x + offsetX, y + offsetY, z + offsetZ + 1);
												if (!voxelItem.IsValid() || voxelItem.GetVoxelData().Transparency == ETransparency::Transparent)
												{
													SetVoxelComplex(FIndex(x + offsetX, y + offsetY, z + offsetZ + 1), FVoxelItem(EVoxelType::Oak_Leaves));
												}
											}
										}
									}
								}
							}
							break;
						}
						default: break;
					}
					SetVoxelSample(voxelIndex, FVoxelItem(voxelType));
				}
			}
		}
	}
}

void ADWVoxelChunk::GenerateMap()
{
	for (auto& iter : VoxelMap)
	{
		const FVoxelItem& voxelItem = iter.Value;
		if (voxelItem.IsValid())
		{
			switch (voxelItem.GetVoxelData().Transparency)
			{
				case ETransparency::Solid:
				{
					if (GetSolidMesh())
					{
						GetSolidMesh()->BuildVoxel(voxelItem);
					}
					break;
				}
				case ETransparency::SemiTransparent:
				{
					if (GetSemiMesh())
					{
						GetSemiMesh()->BuildVoxel(voxelItem);
					}
					break;
				}
				case ETransparency::Transparent:
				{
					if (GetTransMesh())
					{
						GetTransMesh()->BuildVoxel(voxelItem);
					}
					break;
				}
			}
		}
	}
}

void ADWVoxelChunk::SpawnActors()
{
	if (AVoxelModule::GetWorldData().IsExistChunkData(Index))
	{
		FDWChunkSaveData ChunkSaveData = AVoxelModule::GetWorldData().GetDWVoxelChunkData(Index);
		for (int32 i = 0; i < ChunkSaveData.PickUpDatas.Num(); i++)
		{
			SpawnPickUp(ChunkSaveData.PickUpDatas[i]);
		}
		for (int32 i = 0; i < ChunkSaveData.CharacterDatas.Num(); i++)
		{
			SpawnCharacter(ChunkSaveData.CharacterDatas[i]);
		}
		for (int32 i = 0; i < ChunkSaveData.VitalityDatas.Num(); i++)
		{
			SpawnVitality(ChunkSaveData.VitalityDatas[i]);
		}
	}
	else if(SolidMesh || SemiMesh)
	{
		if (FIndex::Distance(Index, AVoxelModule::GetWorldData().LastVitalityRaceIndex) > 250.f / AVoxelModule::GetWorldData().VitalityRaceDensity)
		{
			auto raceData = UDWHelper::RandomVitalityRaceData();
			for (int32 i = 0; i < raceData.Items.Num(); i++)
			{
				auto vitalityItem = raceData.Items[i];
				auto vitalityData = UDWHelper::LoadVitalityData(vitalityItem.ID);
				for (int32 j = 0; j < vitalityItem.Count; j++)
				{
					for (int32 k = 0; k < 10; k++)
					{
						FHitResult hitResult;
						if (AVoxelModule::Get()->ChunkTraceSingle(this, FMath::Max(vitalityData.Range.X, vitalityData.Range.Y) * 0.5f * AVoxelModule::GetWorldData().BlockSize, vitalityData.Range.Z * 0.5f * AVoxelModule::GetWorldData().BlockSize, hitResult))
						{
							auto saveData = FVitalitySaveData();
							saveData.ID = vitalityData.ID;
							saveData.Name = vitalityData.Name.ToString();
							saveData.RaceID = raceData.ID.ToString();
							saveData.Level = vitalityItem.Level;
							saveData.SpawnLocation = hitResult.Location;
							saveData.SpawnRotation = FRotator(0, FMath::RandRange(0, 360), 0);
							SpawnVitality(saveData);
							AVoxelModule::GetWorldData().LastVitalityRaceIndex = Index;
							break;
						}
					}
				}
			}
		}
		if (FIndex::Distance(Index, AVoxelModule::GetWorldData().LastCharacterRaceIndex) > 300.f / AVoxelModule::GetWorldData().CharacterRaceDensity)
		{
			ACharacterBase* captain = nullptr;
			auto raceData = UDWHelper::RandomCharacterRaceData();
			for (int32 i = 0; i < raceData.Items.Num(); i++)
			{
				auto characterItem = raceData.Items[i];
				auto characterData = UDWHelper::LoadCharacterData(characterItem.ID);
				for (int32 j = 0; j < characterItem.Count; j++)
				{
					for (int32 k = 0; k < 10; k++)
					{
						FHitResult hitResult;
						if (AVoxelModule::Get()->DWVoxelChunkTraceSingle(this, FMath::Max(characterData.Range.X, characterData.Range.Y) * 0.5f * AVoxelModule::GetWorldData().BlockSize, characterData.Range.Z * 0.5f * AVoxelModule::GetWorldData().BlockSize, hitResult))
						{
							auto saveData = FCharacterSaveData();
							saveData.ID = characterData.ID;
							saveData.Name = characterData.Name.ToString();
							saveData.RaceID = raceData.ID.ToString();
							saveData.Level = characterItem.Level;
							saveData.SpawnLocation = hitResult.Location;
							saveData.SpawnRotation = FRotator(0, FMath::RandRange(0, 360), 0);
							if(auto character = SpawnCharacter(saveData))
							{
								if (captain == nullptr)
								{
									captain = character;
									AVoxelModule::Get()->CreateTeam(captain);
								}
								else
								{
									captain->AddTeamMate(character);
								}
							}
							AVoxelModule::GetWorldData().LastCharacterRaceIndex = Index;
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

void ADWVoxelChunk::GenerateNeighbors(FIndex InIndex)
{
	GenerateNeighbors(InIndex.X, InIndex.Y, InIndex.Z);
}

void ADWVoxelChunk::GenerateNeighbors(int32 InX, int32 InY, int32 InZ)
{
	if (InX <= 0 && Neighbors[(int32)EDirection::Back] != nullptr) {
		Neighbors[(int32)EDirection::Back]->Generate();
	}
	else if (InX >= AVoxelModule::GetWorldData().ChunkSize - 1 && Neighbors[(int32)EDirection::Forward] != nullptr) {
		Neighbors[(int32)EDirection::Forward]->Generate();
	}
	if (InY <= 0 && Neighbors[(int32)EDirection::Left] != nullptr) {
		Neighbors[(int32)EDirection::Left]->Generate();
	}
	else if (InY >= AVoxelModule::GetWorldData().ChunkSize - 1 && Neighbors[(int32)EDirection::Right] != nullptr) {
		Neighbors[(int32)EDirection::Right]->Generate();
	}
	if (InZ <= 0 && Neighbors[(int32)EDirection::Down] != nullptr) {
		Neighbors[(int32)EDirection::Down]->Generate();
	}
	else if (InZ >= AVoxelModule::GetWorldData().ChunkSize - 1 && Neighbors[(int32)EDirection::Up] != nullptr) {
		Neighbors[(int32)EDirection::Up]->Generate();
	}
}

void ADWVoxelChunk::UpdateNeighbors()
{
	for (int32 i = 0; i < 6; i++)
	{
		Neighbors[i] = AVoxelModule::Get()->FindDWVoxelChunk(Index + UDWHelper::DirectionToIndex((EDirection)i));
		if (Neighbors[i] != nullptr)
		{
			Neighbors[i]->Neighbors[(int32)UDWHelper::InvertDirection((EDirection)i)] = this;
		}
	}
}

void ADWVoxelChunk::BreakNeighbors()
{
	for (int32 i = 0; i < 6; i++)
	{
		if (Neighbors[i] != nullptr)
		{
			Neighbors[i]->Neighbors[(int32)UDWHelper::InvertDirection((EDirection)i)] = nullptr;
			Neighbors[i] = nullptr;
		}
	}
}

bool ADWVoxelChunk::IsOnTheDWVoxelChunk(FIndex InIndex) const
{
	return InIndex.X >= 0 && InIndex.X < AVoxelModule::GetWorldData().ChunkSize &&
		InIndex.Y >= 0 && InIndex.Y < AVoxelModule::GetWorldData().ChunkSize &&
		InIndex.Z >= 0 && InIndex.Z < AVoxelModule::GetWorldData().ChunkSize;
}

bool ADWVoxelChunk::IsOnTheDWVoxelChunk(FVector InLocation) const
{
	return InLocation.X >= GetActorLocation().X && InLocation.X < GetActorLocation().X + AVoxelModule::GetWorldData().GetDWVoxelChunkLength() &&
			InLocation.Y >= GetActorLocation().Y && InLocation.Y < GetActorLocation().Y + AVoxelModule::GetWorldData().GetDWVoxelChunkLength() &&
			InLocation.Z >= GetActorLocation().Z && InLocation.Z < GetActorLocation().Z + AVoxelModule::GetWorldData().GetDWVoxelChunkLength();
}

FIndex ADWVoxelChunk::LocationToIndex(FVector InLocation, bool bWorldSpace /*= true*/) const
{
	const FVector point = (!bWorldSpace ? InLocation : GetActorTransform().InverseTransformPosition(InLocation)) / AVoxelModule::GetWorldData().BlockSize;

	FIndex index;
	index.X = FMath::FloorToInt(point.X);
	index.Y = FMath::FloorToInt(point.Y);
	index.Z = FMath::FloorToInt(point.Z);

	return index;
}

FVector ADWVoxelChunk::IndexToLocation(FIndex InIndex, bool bWorldSpace /*= true*/) const
{
	const FVector localPoint = InIndex.ToVector() * AVoxelModule::GetWorldData().BlockSize;
	if (!bWorldSpace) return localPoint;
	return GetActorTransform().TransformPosition(localPoint);
}

FIndex ADWVoxelChunk::LocalIndexToWorld(FIndex InIndex) const
{
	return InIndex + Index * AVoxelModule::GetWorldData().ChunkSize;
}

FIndex ADWVoxelChunk::WorldIndexToLocal(FIndex InIndex) const
{
	return InIndex - Index * AVoxelModule::GetWorldData().ChunkSize;
}

UVoxel* ADWVoxelChunk::GetVoxel(FIndex InIndex)
{
	return GetVoxel(InIndex.X, InIndex.Y, InIndex.Z);
}

UVoxel* ADWVoxelChunk::GetVoxel(int32 InX, int32 InY, int32 InZ)
{
	return GetVoxelItem(InX, InY, InZ).GetVoxel();
}

FVoxelItem& ADWVoxelChunk::GetVoxelItem(FIndex InIndex)
{
	return GetVoxelItem(InIndex.X, InIndex.Y, InIndex.Z);
}

FVoxelItem& ADWVoxelChunk::GetVoxelItem(int32 InX, int32 InY, int32 InZ)
{
	if (InX < 0) {
		if (Neighbors[(int32)EDirection::Back] != nullptr)
			return Neighbors[(int32)EDirection::Back]->GetVoxelItem(InX + AVoxelModule::GetWorldData().ChunkSize, InY, InZ);
		return FVoxelItem::UnknownVoxel;
	}
	else if (InX >= AVoxelModule::GetWorldData().ChunkSize) {
		if (Neighbors[(int32)EDirection::Forward] != nullptr)
			return Neighbors[(int32)EDirection::Forward]->GetVoxelItem(InX - AVoxelModule::GetWorldData().ChunkSize, InY, InZ);
		return FVoxelItem::UnknownVoxel;
	}
	else if (InY < 0) {
		if (Neighbors[(int32)EDirection::Left] != nullptr)
			return Neighbors[(int32)EDirection::Left]->GetVoxelItem(InX, InY + AVoxelModule::GetWorldData().ChunkSize, InZ);
		return FVoxelItem::UnknownVoxel;
	}
	else if (InY >= AVoxelModule::GetWorldData().ChunkSize) {
		if (Neighbors[(int32)EDirection::Right] != nullptr)
			return Neighbors[(int32)EDirection::Right]->GetVoxelItem(InX, InY - AVoxelModule::GetWorldData().ChunkSize, InZ);
		return FVoxelItem::UnknownVoxel;
	}
	else if (InZ < 0) {
		if (Neighbors[(int32)EDirection::Down] != nullptr)
			return Neighbors[(int32)EDirection::Down]->GetVoxelItem(InX, InY, InZ + AVoxelModule::GetWorldData().ChunkSize);
		else if (Index.Z > 0)
			return FVoxelItem::UnknownVoxel;
	}
	else if (InZ >= AVoxelModule::GetWorldData().ChunkSize) {
		if (Neighbors[(int32)EDirection::Up] != nullptr)
			return Neighbors[(int32)EDirection::Up]->GetVoxelItem(InX, InY, InZ - AVoxelModule::GetWorldData().ChunkSize);
		else if (Index.Z < AVoxelModule::GetWorldData().DWVoxelChunkHeightRange)
			return FVoxelItem::UnknownVoxel;
	}
	else if (VoxelMap.Contains(FIndex(InX, InY, InZ))) {
		return VoxelMap[FIndex(InX, InY, InZ)];
	}
	return FVoxelItem::EmptyVoxel;
}

bool ADWVoxelChunk::CheckVoxel(FIndex InIndex, FVector InRange/* = FVector::OneVector*/, bool bIgnoreTransparent /*= true*/)
{
	if (InRange != FVector::OneVector)
	{
		for (int32 x = 0; x < InRange.X; x++)
		{
			for (int32 y = 0; y < InRange.Y; y++)
			{
				for (int32 z = 0; z < InRange.Z; z++)
				{
					const FVoxelItem& voxelItem = GetVoxelItem(InIndex + FIndex(x, y, z));
					if (voxelItem.IsValid() && (!bIgnoreTransparent || voxelItem.GetVoxelData().Transparency != ETransparency::Transparent))
					{
						return true;
					}
				}
			}
		}
	}
	else
	{
		const FVoxelItem& voxelItem = GetVoxelItem(InIndex);
		if (voxelItem.IsValid() && (!bIgnoreTransparent || voxelItem.GetVoxelData().Transparency != ETransparency::Transparent))
		{
			return true;
		}
	}
	return false;
}

bool ADWVoxelChunk::CheckVoxel(int32 InX, int32 InY, int32 InZ, FVector InRange/* = FVector::OneVector*/, bool bIgnoreTransparent /*= true*/)
{
	return CheckVoxel(FIndex(InX, InY, InZ), InRange, bIgnoreTransparent);
}

bool ADWVoxelChunk::CheckAdjacent(const FVoxelItem& InVoxelItem, EDirection InDirection)
{
	if(InDirection == EDirection::Down && LocalIndexToWorld(InVoxelItem.Index).Z == 0) return false;

	UVoxelAssetBase voxelData = InVoxelItem.GetVoxelData();
	
	FVoxelItem& adjacentItem = GetVoxelItem(UDWHelper::GetAdjacentIndex(InVoxelItem.Index, InDirection, InVoxelItem.Rotation));
	UVoxelAssetBase adjacentData = adjacentItem.GetVoxelData();

	if (adjacentItem.IsValid())
	{
		if(voxelData.Range * InVoxelItem.Scale == adjacentData.Range * adjacentItem.Scale)
		{
			switch (voxelData.Transparency)
			{
				case ETransparency::Solid:
				{
					switch (adjacentData.Transparency)
					{
						case ETransparency::Solid:
						{
							return false;
						}
						default: break;
					}
					break;
				}
				case ETransparency::SemiTransparent:
				{
					switch (adjacentData.Transparency)
					{
						case ETransparency::SemiTransparent:
						{
							if (voxelData.VoxelType == adjacentData.VoxelType)
							{
								switch (voxelData.VoxelType)
								{
									case EVoxelType::Oak_Leaves:
									case EVoxelType::Birch_Leaves:
									case EVoxelType::Ice:
									case EVoxelType::Glass:
									{
										return false;
									}
									default: break;
								}
							}
							break;
						}
						default: break;
					}
					break;
				}
				case ETransparency::Transparent:
				{
					switch (adjacentData.Transparency)
					{
						case ETransparency::Solid:
						case ETransparency::SemiTransparent:
						{
							return false;
						}
						case ETransparency::Transparent:
						{
							if (voxelData.VoxelType == adjacentData.VoxelType)
							{
								switch (voxelData.VoxelType)
								{
									case EVoxelType::Water:
									{
										return false;
									}
									default: break;
								}
							}
							break;
						}
					}
					break;
				}
			}
		}
	}
	else if(adjacentItem == FVoxelItem::UnknownVoxel)
	{
		return false;
	}
	return true;
}

bool ADWVoxelChunk::CheckNeighbors(const FVoxelItem& InVoxelItem, EVoxelType InVoxelType, bool bIgnoreBottom, int32 InDistance)
{
	const FVector range = InVoxelItem.GetVoxelData().GetCeilRange(InVoxelItem.Rotation, InVoxelItem.Scale);
	for (int32 x = -InDistance; x < range.X + InDistance; x++)
	{
		for (int32 y = -InDistance; y < range.Y + InDistance; y++)
		{
			for (int32 z = bIgnoreBottom ? 0 : -InDistance; z < range.Z + InDistance; z++)
			{
				FVoxelItem& neighborItem = GetVoxelItem(Index + FIndex(x, y, z));
				if (neighborItem.IsValid())
				{
					if (neighborItem.GetVoxelData().VoxelType == InVoxelType)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool ADWVoxelChunk::SetVoxelSample(FIndex InIndex, const FVoxelItem& InVoxelItem, bool bGenerateMesh)
{
	bool RetValue = false;

	if (InVoxelItem.IsValid())
	{
		if (!CheckVoxel(InIndex))
		{
			FVoxelItem VoxelItem;
			VoxelItem.ID = InVoxelItem.ID;
			VoxelItem.Index = InIndex;
			VoxelItem.Owner = this;
			if (!VoxelMap.Contains(InIndex))
			{
				VoxelMap.Add(InIndex, VoxelItem);
			}
			else
			{
				//DestroyVoxel(InIndex);
				VoxelMap[InIndex] = VoxelItem;
			}
			RetValue = true;
		}
	}
	else if (VoxelMap.Contains(InIndex))
	{
		VoxelMap.Remove(InIndex);
		RetValue = true;
	}

	if (RetValue && bGenerateMesh)
	{
		Generate();
		GenerateNeighbors(InIndex);
	}

	return RetValue;
}

bool ADWVoxelChunk::SetVoxelSample(int32 InX, int32 InY, int32 InZ, const FVoxelItem& InVoxelItem, bool bGenerateMesh)
{
	return SetVoxelSample(FIndex(InX, InY, InZ), InVoxelItem, bGenerateMesh);
}

bool ADWVoxelChunk::SetVoxelComplex(FIndex InIndex, const FVoxelItem& InVoxelItem, bool bGenerateMesh)
{
	return SetVoxelComplex(InIndex.X, InIndex.Y, InIndex.Z, InVoxelItem, bGenerateMesh);
}

bool ADWVoxelChunk::SetVoxelComplex(int32 InX, int32 InY, int32 InZ, const FVoxelItem& InVoxelItem, bool bGenerateMesh)
{
	if (InX < 0) {
		if (Neighbors[(int32)EDirection::Back] != nullptr)
			return Neighbors[(int32)EDirection::Back]->SetVoxelComplex(InX + AVoxelModule::GetWorldData().ChunkSize, InY, InZ, InVoxelItem, bGenerateMesh);
	}
	else if (InX >= AVoxelModule::GetWorldData().ChunkSize) {
		if (Neighbors[(int32)EDirection::Forward] != nullptr)
			return Neighbors[(int32)EDirection::Forward]->SetVoxelComplex(InX - AVoxelModule::GetWorldData().ChunkSize, InY, InZ, InVoxelItem, bGenerateMesh);
	}
	else if (InY < 0) {
		if (Neighbors[(int32)EDirection::Left] != nullptr)
			return Neighbors[(int32)EDirection::Left]->SetVoxelComplex(InX, InY + AVoxelModule::GetWorldData().ChunkSize, InZ, InVoxelItem, bGenerateMesh);
	}
	else if (InY >= AVoxelModule::GetWorldData().ChunkSize) {
		if (Neighbors[(int32)EDirection::Right] != nullptr)
			return Neighbors[(int32)EDirection::Right]->SetVoxelComplex(InX, InY - AVoxelModule::GetWorldData().ChunkSize, InZ, InVoxelItem, bGenerateMesh);
	}
	else if (InZ < 0) {
		if (Neighbors[(int32)EDirection::Down] != nullptr)
			return Neighbors[(int32)EDirection::Down]->SetVoxelComplex(InX, InY, InZ + AVoxelModule::GetWorldData().ChunkSize, InVoxelItem, bGenerateMesh);
	}
	else if (InZ >= AVoxelModule::GetWorldData().ChunkSize) {
		if (Neighbors[(int32)EDirection::Up] == nullptr)
			AVoxelModule::Get()->SpawnDWVoxelChunk(Index + UDWHelper::DirectionToIndex(EDirection::Up), !bGenerateMesh);
		if (Neighbors[(int32)EDirection::Up] != nullptr)
			return Neighbors[(int32)EDirection::Up]->SetVoxelComplex(InX, InY, InZ - AVoxelModule::GetWorldData().ChunkSize, InVoxelItem, bGenerateMesh);
	}
	else {
		if (InVoxelItem.GetVoxelData().IsComplex())
		{
			if (!InVoxelItem.IsEmpty())
			{
				const FVector range = InVoxelItem.GetVoxelData().GetCeilRange(InVoxelItem.Rotation, InVoxelItem.Scale);
				if (!CheckVoxel(InX, InY, InZ, range))
				{
					for (int32 x = 0; x < range.X; x++)
					{
						for (int32 y = 0; y < range.Y; y++)
						{
							for (int32 z = 0; z < range.Z; z++)
							{
								FIndex index = FIndex(InX + x, InY + y, InZ + z);
								SetVoxelSample(index, InVoxelItem, bGenerateMesh);
							}
						}
					}
					return true;
				}
			}
			else
			{
				const FVoxelItem& voxelItem = GetVoxelItem(InX, InY, InZ);
				if(voxelItem.IsValid())
				{
					FVector range = voxelItem.GetVoxelData().GetCeilRange(voxelItem.Rotation, voxelItem.Scale);
					for (int32 x = 0; x < range.X; x++)
					{
						for (int32 y = 0; y < range.Y; y++)
						{
							for (int32 z = 0; z < range.Z; z++)
							{
								FIndex index = voxelItem.Index + FIndex(InX, InY, InZ);
								if (VoxelMap.Contains(index))
								{
									VoxelMap.Remove(index);
								}
							}
						}
					}
					return true;
				}
			}
		}
		else
		{
			return SetVoxelSample(InX, InY, InZ, InVoxelItem, bGenerateMesh);
		}
	}
	return false;
}

bool ADWVoxelChunk::GenerateVoxel(FIndex InIndex, const FVoxelItem& InVoxelItem)
{
	if(SetVoxelComplex(InIndex, InVoxelItem, true))
	{
		SpawnAuxiliary(GetVoxelItem(InIndex));
		const UVoxelAssetBase VoxelData = InVoxelItem.GetVoxelData();
		if(VoxelData.GenerateSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, VoxelData.GenerateSound, IndexToLocation(InIndex));
		}
		return true;
	}
	return false;
}

bool ADWVoxelChunk::DestroyVoxel(FIndex InIndex)
{
	return DestroyVoxel(GetVoxelItem(InIndex));
}

bool ADWVoxelChunk::DestroyVoxel(const FVoxelItem& InVoxelItem)
{
	if (SetVoxelComplex(InVoxelItem.Index, FVoxelItem::EmptyVoxel, true))
	{
		const UVoxelAssetBase voxelData = InVoxelItem.GetVoxelData();
		if(voxelData.GenerateSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, voxelData.GenerateSound, IndexToLocation(InVoxelItem.Index));
		}
		DestroyAuxiliary(InVoxelItem.Auxiliary);

		FVector range = voxelData.GetCeilRange(InVoxelItem.Rotation, InVoxelItem.Scale);
		SpawnPickUp(FItem(InVoxelItem.ID, 1), IndexToLocation(InVoxelItem.Index) + range * AVoxelModule::GetWorldData().BlockSize * 0.5f);

		for (int32 x = 0; x < range.X; x++)
		{
			for (int32 y = 0; y < range.Y; y++)
			{
				FIndex tmpIndex = InVoxelItem.Index + FIndex(x, y, range.Z);
				const FVoxelItem& tmpVoxelItem = GetVoxelItem(tmpIndex);
				if (tmpVoxelItem.IsValid())
				{
					UVoxelAssetBase tmpVoxelData = tmpVoxelItem.GetVoxelData();
					if(tmpVoxelData.Transparency == ETransparency::Transparent && tmpVoxelData.VoxelType != EVoxelType::Water)
					{
						DestroyVoxel(tmpVoxelItem);
					}
				}
			}
		}
		if (CheckNeighbors(InVoxelItem, EVoxelType::Water, true))
		{
			for (int32 x = 0; x < range.X; x++)
			{
				for (int32 y = 0; y < range.Y; y++)
				{
					for (int32 z = 0; z < range.Z; z++)
					{
						SetVoxelComplex(InVoxelItem.Index + FIndex(x, y, z), FVoxelItem(EVoxelType::Water), true);
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool ADWVoxelChunk::ReplaceVoxel(const FVoxelItem& InOldVoxelItem, const FVoxelItem& InNewVoxelItem)
{
	if (SetVoxelComplex(InOldVoxelItem.Index, InNewVoxelItem, true))
	{
		const UVoxelAssetBase VoxelData = InNewVoxelItem.GetVoxelData();
		SpawnAuxiliary(GetVoxelItem(InOldVoxelItem.Index));
		if(VoxelData.GenerateSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, VoxelData.GenerateSound, IndexToLocation(InNewVoxelItem.Index));
		}
		return true;
	}
	return false;
}

AVoxelAuxiliary* ADWVoxelChunk::SpawnAuxiliary(FVoxelItem& InVoxelItem)
{
	const UVoxelAssetBase VoxelData = InVoxelItem.GetVoxelData();
	if (VoxelMap.Contains(InVoxelItem.Index) && VoxelData.AuxiliaryClass)
	{
		FActorSpawnParameters spawnParams = FActorSpawnParameters();
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FVector Location = IndexToLocation(InVoxelItem.Index, true) + VoxelData.GetCeilRange() * AVoxelModule::GetWorldData().BlockSize * 0.5f;
		if (AVoxelAuxiliary* Auxiliary = GetWorld()->SpawnActor<AVoxelAuxiliary>(VoxelData.AuxiliaryClass, Location, InVoxelItem.Rotation, spawnParams))
		{
			InVoxelItem.Auxiliary = Auxiliary;
			Auxiliary->Initialize(this, InVoxelItem.Index);
			Auxiliary->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			//SetVoxelSample(InVoxelItem.Index, InVoxelItem, false, false);
			return Auxiliary;
		}
	}
	return nullptr;
}

void ADWVoxelChunk::DestroyAuxiliary(AVoxelAuxiliary* InAuxiliary)
{
	if(!InAuxiliary || !InAuxiliary->IsValidLowLevel()) return;

	if(VoxelMap.Contains(InAuxiliary->GetVoxelItem().Index))
	{
		InAuxiliary->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		InAuxiliary->Destroy();
		InAuxiliary->GetVoxelItem().Auxiliary = nullptr;
	}
}

APickUp* ADWVoxelChunk::SpawnPickUp(FItem InItem, FVector InLocation)
{
	if (InItem == FItem::Empty) return nullptr;

	APickUp* pickUpItem = nullptr;
	
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	switch (InItem.GetData().Type)
	{
		case EItemType::Voxel:
		{
			pickUpItem = GetWorld()->SpawnActor<APickUpVoxel>(InLocation, FRotator::ZeroRotator, spawnParams);
			break;
		}
		case EItemType::Equip:
		{
			pickUpItem = GetWorld()->SpawnActor<APickUpEquip>(InLocation, FRotator::ZeroRotator, spawnParams);
			break;
		}
		case EItemType::Prop:
		{
			pickUpItem = GetWorld()->SpawnActor<APickUpProp>(InLocation, FRotator::ZeroRotator, spawnParams);
			break;
		}
		default: break;
	}
	
	if (pickUpItem != nullptr)
	{
		AttachPickUp(pickUpItem);
		pickUpItem->Initialize(InItem);
	}
	return pickUpItem;
}

APickUp* ADWVoxelChunk::SpawnPickUp(FPickUpSaveData InPickUpData)
{
	return SpawnPickUp(InPickUpData.Item, InPickUpData.Location);
}

void ADWVoxelChunk::AttachPickUp(APickUp* InPickUp)
{
	if(!InPickUp || !InPickUp->IsValidLowLevel() || PickUps.Contains(InPickUp)) return;

	InPickUp->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	InPickUp->OwnerDWVoxelChunk = this;
	PickUps.Add(InPickUp);
}

void ADWVoxelChunk::DetachPickUp(APickUp* InPickUp)
{
	if(!InPickUp || !InPickUp->IsValidLowLevel() || !PickUps.Contains(InPickUp)) return;

	InPickUp->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InPickUp->OwnerDWVoxelChunk = nullptr;
	PickUps.Remove(InPickUp);
}

void ADWVoxelChunk::DestroyPickUp(APickUp* InPickUp)
{
	if(!InPickUp || !InPickUp->IsValidLowLevel()) return;

	if (PickUps.Contains(InPickUp))
	{
		PickUps.Remove(InPickUp);
		InPickUp->Destroy();
	}
}

ACharacterBase* ADWVoxelChunk::SpawnCharacter(FCharacterSaveData InSaveData)
{
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ACharacterBase* character = GetWorld()->SpawnActor<ACharacterBase>(InSaveData.GetCharacterData().Class, spawnParams))
	{
		character->LoadData(&InSaveData);
		character->SpawnDefaultController();
		AttachCharacter(character);
		return character;
	}
	return nullptr;
}

void ADWVoxelChunk::AttachCharacter(ACharacterBase* InCharacter)
{
	if(!InCharacter || !InCharacter->IsValidLowLevel() || Characters.Contains(InCharacter)) return;

	// InCharacter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	InCharacter->SetOwnerDWVoxelChunk(this);
	Characters.Add(InCharacter);
}

void ADWVoxelChunk::DetachCharacter(ACharacterBase* InCharacter)
{
	if(!InCharacter || !InCharacter->IsValidLowLevel() || !Characters.Contains(InCharacter)) return;

	// InCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InCharacter->SetOwnerDWVoxelChunk(nullptr);
	Characters.Remove(InCharacter);
}

void ADWVoxelChunk::DestroyCharacter(ACharacterBase* InCharacter)
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
	if (ADWVitality* Vitality = GetWorld()->SpawnActor<ADWVitality>(InSaveData.GetVitalityData().Class, spawnParams))
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
	InVitality->SetOwnerDWVoxelChunk(this);
	Vitalitys.Add(InVitality);
}

void ADWVoxelChunk::DetachVitality(ADWVitality* InVitality)
{
	if(!InVitality || !InVitality->IsValidLowLevel() || !Vitalitys.Contains(InVitality)) return;

	InVitality->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	InVitality->SetOwnerDWVoxelChunk(nullptr);
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
