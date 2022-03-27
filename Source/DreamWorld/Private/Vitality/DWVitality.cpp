	// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitality.h"

#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Ability/Vitality/DWVitalityAttributeSet.h"
#include "Ability/Vitality/VitalityAssetBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/DWCharacter.h"
#include "Interaction/Components/VitalityInteractionComponent.h"
#include "Inventory/VitalityInventory.h"
#include "Vitality/DWVitalityAsset.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Widget/Components/WidgetVitalityHPComponent.h"
#include "Widget/World/WidgetVitalityHP.h"

	// Sets default values
ADWVitality::ADWVitality()
{
	WidgetVitalityHP = CreateDefaultSubobject<UWidgetVitalityHPComponent>(FName("WidgetVitalityHP"));
	WidgetVitalityHP->SetupAttachment(RootComponent);
	WidgetVitalityHP->SetRelativeLocation(FVector(0, 0, 50));

	Interaction = CreateDefaultSubobject<UAbilityVitalityInteractionComponent>(FName("Interaction"));
	Interaction->SetupAttachment(RootComponent);
	Interaction->SetRelativeLocation(FVector(0, 0, 0));

	AbilitySystem = CreateDefaultSubobject<UDWAbilitySystemComponent>(FName("AbilitySystem"));

	AttributeSet = CreateDefaultSubobject<UDWVitalityAttributeSet>(FName("AttributeSet"));
	
	Inventory = CreateDefaultSubobject<UAbilityVitalityInventory>(FName("Inventory"));

	OwnerChunk = nullptr;
}

// Called when the game starts or when spawned
void ADWVitality::BeginPlay()
{
	Super::BeginPlay();

	if (GetWidgetVitalityHPWidget() && !GetWidgetVitalityHPWidget()->GetOwnerObject())
	{
		GetWidgetVitalityHPWidget()->SetOwnerObject(this);
	}

	Spawn();
}

// Called every frame
void ADWVitality::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bDead) return;

	Inventory->Refresh(DeltaTime);
}

void ADWVitality::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);
}

void ADWVitality::LoadData(FSaveData* InSaveData)
{
	auto SaveData = *static_cast<FVitalitySaveData*>(InSaveData);
	if (SaveData.bSaved)
	{
		AssetID = SaveData.ID;
		SetNameV(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelV(SaveData.Level);
		SetEXP(SaveData.EXP);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		Inventory->LoadData(SaveData.InventoryData, this);
	}
	else
	{
		AssetID = SaveData.ID;
		SetNameV(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelV(SaveData.Level);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		const UDWVitalityAsset& vitalityData = GetVitalityData<UDWVitalityAsset>();
		if(vitalityData.IsValid())
		{
			SaveData.InventoryData = vitalityData.InventoryData;
		}

		// const auto ItemDatas = UDWHelper::LoadItemDatas();
		// if(ItemDatas.Num() > 0 && FMath::FRand() < 0.2f)
		// {
		// 	SaveData.InventoryData.Items.Add(FItem(ItemDatas[FMath::RandRange(0, ItemDatas.Num() - 1)].ID, 1));
		// }
		
		Inventory->LoadData(SaveData.InventoryData, this);
	}
}

FSaveData* ADWVitality::ToData(bool bSaved)
{
	static FVitalitySaveData SaveData;

	SaveData.bSaved = bSaved;

	SaveData.ID = AssetID;
	SaveData.Name = Name;
	SaveData.RaceID = RaceID;
	SaveData.Level = Level;
	SaveData.EXP = EXP;
	
	SaveData.InventoryData = Inventory->ToData();

	SaveData.SpawnLocation = GetActorLocation();
	SaveData.SpawnRotation = GetActorRotation();

	return &SaveData;
}

void ADWVitality::ResetData(bool bRefresh)
{
	Super::ResetData(bRefresh);
}

void ADWVitality::RefreshData()
{
	Super::RefreshData();
}

void ADWVitality::Death(AActor* InKiller /*= nullptr*/)
{
	if (!bDead)
	{
		Inventory->DiscardAllItem();
		OwnerChunk->DestroySceneObject(this);
	}
	Super::Death(InKiller);
}

void ADWVitality::Spawn()
{
	Super::Spawn();
}

void ADWVitality::Revive()
{
	Super::Revive();
}

bool ADWVitality::CanInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			if(bDead)
			{
				return true;
			}
			break;
		}
		default: return true;
	}
	return false;
}

void ADWVitality::OnInteract(IInteractionInterface* InInteractionTarget, EInteractAction InInteractAction)
{
	switch (InInteractAction)
	{
		case EInteractAction::Revive:
		{
			Revive();
			break;
		}
		default: break;
	}
}

UWidgetVitalityHP* ADWVitality::GetWidgetVitalityHPWidget() const
{
	if (WidgetVitalityHP->GetUserWidgetObject())
	{
		return Cast<UWidgetVitalityHP>(WidgetVitalityHP->GetUserWidgetObject());
	}
	return nullptr;
}

UInteractionComponent* ADWVitality::GetInteractionComponent() const
{
	return Interaction;
}

void ADWVitality::HandleDamage(EDamageType DamageType, const float LocalDamageDone, bool bHasCrited, FHitResult HitResult, const FGameplayTagContainer& SourceTags, AActor* SourceActor)
{
	Super::HandleDamage(DamageType, LocalDamageDone, bHasCrited, HitResult, SourceTags, SourceActor);
	
	if(SourceActor && SourceActor != this)
	{
		if(ADWCharacter* SourceCharacter = Cast<ADWCharacter>(SourceActor))
		{
			if(DamageType == EDamageType::Physics)
			{
				SourceCharacter->ModifyHealth(LocalDamageDone * SourceCharacter->GetAttackStealRate());
			}
		}
	}
}

void ADWVitality::HandleNameChanged(const FString& NewValue)
{
	Super::HandleNameChanged(NewValue);
	
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleRaceIDChanged(const FString& NewValue)
{
	Super::HandleRaceIDChanged(NewValue);
	
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleLevelChanged(int32 NewValue, int32 DeltaValue /*= 0*/)
{
	Super::HandleLevelChanged(NewValue, DeltaValue);
	
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleEXPChanged(int32 NewValue, int32 DeltaValue /*= 0*/)
{
	Super::HandleEXPChanged(NewValue, DeltaValue);
	
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleHealthChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	Super::HandleHealthChanged(NewValue, DeltaValue);
	
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHealthPercent(NewValue, GetMaxHealth());
	}
}

void ADWVitality::HandleMaxHealthChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	Super::HandleMaxHealthChanged(NewValue, DeltaValue);
	
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHealthPercent(NewValue, GetMaxHealth());
	}
}
