	// Fill out your copyright notice in the Description page of Project Settings.


#include "Vitality/DWVitality.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/DWCharacter.h"
#include "Widget/Other/WidgetWorldText.h"
#include "Widget/Other/WidgetVitalityHP.h"
#include "Widget/Components/WidgetVitalityHPComponent.h"
#include "Widget/Components/WorldTextComponent.h"
#include "World/Chunk.h"
#include "Inventory/Inventory.h"
#include "Abilities/AbilitySystemComponentBase.h"
#include "Abilities/AttributeSetBase.h"
#include "Abilities/GameplayAbilityBase.h"
#include "Ability/Components/DWAbilitySystemComponent.h"
#include "Ability/Vitality/DWVitalityAttributeSet.h"
#include "Interaction/Components/VitalityInteractionComponent.h"
#include "Inventory/VitalityInventory.h"
#include "Voxel/DWVoxelChunk.h"
#include "Voxel/Chunks/VoxelChunk.h"

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

	AbilitySystem->InitAbilityActorInfo(this, this);

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
		ID = SaveData.ID;
		SetNameC(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelC(SaveData.Level);
		SetEXP(SaveData.EXP);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		Inventory->LoadData(SaveData.InventoryData, this);
	}
	else
	{
		ID = SaveData.ID;
		SetNameC(SaveData.Name);
		SetRaceID(SaveData.RaceID);
		SetLevelC(SaveData.Level);

		SetActorLocation(SaveData.SpawnLocation);
		SetActorRotation(SaveData.SpawnRotation);

		const FVitalityData vitalityData = GetVitalityData();
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

	SaveData.ID = ID;
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
		bDead = true;
		if(IAbilityVitalityInterface* InVitality = Cast<IAbilityVitalityInterface>(InKiller))
		{
			InVitality->ModifyEXP(GetTotalEXP());
		}
		SetEXP(0);
		SetHealth(0.f);
		Inventory->DiscardAllItem();
		OwnerChunk->DestroyVitality(this);
	}
}

void ADWVitality::Spawn()
{
	ResetData();
	SetHealth(GetMaxHealth());
}

void ADWVitality::Revive()
{
	if (bDead)
	{
		ResetData();
		SetHealth(GetMaxHealth());
	}
}

bool ADWVitality::IsDead() const
{
	return bDead;
}

void ADWVitality::SetNameV(const FString& InName)
{
	Name = InName;
	HandleNameChanged(InName);
}

void ADWVitality::SetRaceID(const FString& InRaceID)
{
	RaceID = InRaceID;
	HandleRaceIDChanged(InRaceID);
}

void ADWVitality::SetLevelV(int32 InLevel)
{
	Level = InLevel;
	HandleLevelChanged(InLevel);
}

void ADWVitality::SetEXP(int32 InEXP)
{
	EXP = InEXP;
	HandleEXPChanged(InEXP);
}

int32 ADWVitality::GetMaxEXP() const
{
	int32 MaxEXP = BaseEXP;
	for (int i = 0; i < Level - 1; i++)
	{
		MaxEXP *= EXPFactor;
	}
	return MaxEXP;
}

int32 ADWVitality::GetTotalEXP() const
{
	int32 TotalEXP = BaseEXP;
	for (int i = 0; i < Level - 1; i++)
	{
		TotalEXP += TotalEXP * EXPFactor;
	}
	return EXP + TotalEXP - GetMaxEXP();
}

FString ADWVitality::GetHeadInfo() const
{
	return FString::Printf(TEXT("Lv.%d \"%s\" "), Level, *Name);
}

float ADWVitality::GetHealth() const
{
	return AttributeSet->GetHealth();
}

void ADWVitality::SetHealth(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(AttributeSet->GetHealthAttribute(), EGameplayModOp::Override, InValue);
}

float ADWVitality::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

void ADWVitality::SetMaxHealth(float InValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(AttributeSet->GetMaxHealthAttribute(), EGameplayModOp::Override, InValue);
}

float ADWVitality::GetPhysicsDamage() const
{
	return AttributeSet->GetPhysicsDamage();
}

float ADWVitality::GetMagicDamage() const
{
	return AttributeSet->GetMagicDamage();
}

FVitalityData ADWVitality::GetVitalityData() const
{
	return UDWHelper::LoadVitalityData(ID);
}

void ADWVitality::AddWorldText(FString InContent, EWorldTextType InContentType, EWorldTextStyle InContentStyle)
{
	auto contextHUD = NewObject<UWorldTextComponent>(this);
	contextHUD->RegisterComponent();
	contextHUD->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	contextHUD->SetRelativeLocation(FVector(0, 0, 50));
	contextHUD->SetVisibility(false);
	contextHUD->InitContent(InContent, InContentType, InContentStyle);
}

FGameplayAbilitySpecHandle ADWVitality::AcquireAbility(TSubclassOf<UAbilityBase> InAbility, int32 InLevel /*= 1*/)
{
	if (AbilitySystem && InAbility)
	{
		FGameplayAbilitySpecDef SpecDef = FGameplayAbilitySpecDef();
		SpecDef.Ability = InAbility;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(SpecDef, InLevel);
		return AbilitySystem->GiveAbility(AbilitySpec);
	}
	return FGameplayAbilitySpecHandle();
}

bool ADWVitality::ActiveAbility(FGameplayAbilitySpecHandle SpecHandle, bool bAllowRemoteActivation /*= false*/)
{
	if (AbilitySystem)
	{
		return AbilitySystem->TryActivateAbility(SpecHandle, bAllowRemoteActivation);
	}
	return false;
}

bool ADWVitality::ActiveAbilityByClass(TSubclassOf<UAbilityBase> AbilityClass, bool bAllowRemoteActivation /*= false*/)
{
	if (AbilitySystem)
	{
		return AbilitySystem->TryActivateAbilityByClass(AbilityClass, bAllowRemoteActivation);
	}
	return false;
}

bool ADWVitality::ActiveAbilityByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation /*= false*/)
{
	if (AbilitySystem)
	{
		return AbilitySystem->TryActivateAbilitiesByTag(GameplayTagContainer, bAllowRemoteActivation);
	}
	return false;
}

void ADWVitality::CancelAbility(UAbilityBase* Ability)
{
	if (AbilitySystem)
	{
		AbilitySystem->CancelAbility(Ability);
	}
}

void ADWVitality::CancelAbilityByHandle(const FGameplayAbilitySpecHandle& AbilityHandle)
{
	if (AbilitySystem)
	{
		AbilitySystem->CancelAbilityHandle(AbilityHandle);
	}
}

void ADWVitality::CancelAbilities(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags, UAbilityBase* Ignore/*=nullptr*/)
{
	if (AbilitySystem)
	{
		AbilitySystem->CancelAbilities(&WithTags, &WithoutTags, Ignore);
	}
}

void ADWVitality::CancelAllAbilities(UAbilityBase* Ignore/*=nullptr*/)
{
	if (AbilitySystem)
	{
		AbilitySystem->CancelAllAbilities(Ignore);
	}
}

FActiveGameplayEffectHandle ADWVitality::ApplyEffectByClass(TSubclassOf<UGameplayEffect> EffectClass)
{
	if (AbilitySystem)
	{
		auto effectContext = AbilitySystem->MakeEffectContext();
		effectContext.AddSourceObject(this);
		auto specHandle = AbilitySystem->MakeOutgoingSpec(EffectClass, GetLevelC(), effectContext);
		if (specHandle.IsValid())
		{
			return AbilitySystem->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
		}
	}
	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle ADWVitality::ApplyEffectBySpecHandle(const FGameplayEffectSpecHandle& SpecHandle)
{
	if (AbilitySystem)
	{
		return AbilitySystem->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	return FActiveGameplayEffectHandle();
}

FActiveGameplayEffectHandle ADWVitality::ApplyEffectBySpec(const FGameplayEffectSpec& Spec)
{
	if (AbilitySystem)
	{
		return AbilitySystem->ApplyGameplayEffectSpecToSelf(Spec);
	}
	return FActiveGameplayEffectHandle();
}

bool ADWVitality::RemoveEffect(FActiveGameplayEffectHandle Handle, int32 StacksToRemove/*=-1*/)
{
	if (AbilitySystem)
	{
		return AbilitySystem->RemoveActiveGameplayEffect(Handle, StacksToRemove);
	}
	return false;
}

void ADWVitality::GetActiveAbilities(FGameplayTagContainer AbilityTags, TArray<UAbilityBase*>& ActiveAbilities)
{
	if (AbilitySystem)
	{
		AbilitySystem->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool ADWVitality::GetAbilityInfo(TSubclassOf<UAbilityBase> AbilityClass, FAbilityInfo& OutAbilityInfo)
{
	if (AbilitySystem && AbilityClass != nullptr)
	{
		float Cost = 0;
		float Cooldown = 0;
		EAbilityCostType CostType = EAbilityCostType::None;
		UAbilityBase* Ability = AbilityClass.GetDefaultObject();
		if (Ability->GetCostGameplayEffect()->Modifiers.Num() > 0)
		{
			const FGameplayModifierInfo ModifierInfo = Ability->GetCostGameplayEffect()->Modifiers[0];
			ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1, Cost);
			if (ModifierInfo.Attribute == AttributeSet->GetHealthAttribute())
			{
				CostType = EAbilityCostType::Health;
			}
		}
		Ability->GetCooldownGameplayEffect()->DurationMagnitude.GetStaticMagnitudeIfPossible(1, Cooldown);
		OutAbilityInfo = FAbilityInfo(CostType, Cost, Cooldown);
		return true;
	}
	return false;
}

void ADWVitality::ModifyHealth(float InDeltaValue)
{
	AbilitySystem->ApplyModToAttributeUnsafe(AttributeSet->GetHealthAttribute(), EGameplayModOp::Additive, InDeltaValue);
}

void ADWVitality::ModifyEXP(float InDeltaValue)
{
	const int32 MaxEXP = GetMaxEXP();
	EXP += InDeltaValue;
	if (InDeltaValue > 0.f)
	{
		if (EXP >= MaxEXP)
		{
			Level++;
			EXP -= MaxEXP;
		}
	}
	else
	{
		if (EXP < 0.f)
		{
			EXP = 0.f;
		}
	}
	HandleEXPChanged(EXP);
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
	if (GetHealth() <= 0.f)
	{
		Death(SourceActor);
	}
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
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleRaceIDChanged(const FString& NewValue)
{
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleLevelChanged(int32 NewValue, int32 DeltaValue /*= 0*/)
{
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleEXPChanged(int32 NewValue, int32 DeltaValue /*= 0*/)
{
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHeadInfo(GetHeadInfo());
	}
}

void ADWVitality::HandleHealthChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	if(DeltaValue > 0.f)
	{
		AddWorldText(FString::FromInt(FMath::Abs(DeltaValue)), EWorldTextType::HealthRecover, DeltaValue < GetMaxHealth() ? EWorldTextStyle::Normal : EWorldTextStyle::Stress);
	}
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHealthPercent(NewValue, GetMaxHealth());
	}
}

void ADWVitality::HandleMaxHealthChanged(float NewValue, float DeltaValue /*= 0.f*/)
{
	if (GetWidgetVitalityHPWidget())
	{
		GetWidgetVitalityHPWidget()->SetHealthPercent(NewValue, GetMaxHealth());
	}
}
