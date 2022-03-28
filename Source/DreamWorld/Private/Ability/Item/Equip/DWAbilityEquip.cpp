// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Item/Equip/DWAbilityEquip.h"

#include "Ability/Item/Equip/DWAbilityEquip.h"

#include "Ability/Character/AbilityCharacterBase.h"
#include "Ability/Item/Equip/DWEquipAsset.h"
#include "Ability/Item/Equip/EquipAssetBase.h"
#include "Character/DWCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Global/GlobalBPLibrary.h"
#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventoryEquipSlot.h"

// Sets default values
ADWAbilityEquip::ADWAbilityEquip()
{

}

// Called when the game starts or when spawned
void ADWAbilityEquip::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADWAbilityEquip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADWAbilityEquip::Initialize(AAbilityCharacterBase* InOwnerCharacter)
{
	Super::Initialize(InOwnerCharacter);
	if (GetOwnerCharacter<ADWCharacter>())
	{
		GetOwnerCharacter<ADWCharacter>()->GetInventory()->GetSplitSlots<UInventoryEquipSlot>(ESplitSlotType::Equip)[(int32)GetItemData<UDWEquipAsset>()->PartType];
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, *UGlobalBPLibrary::GetEnumValueAuthoredName(TEXT("EEquipPartType"), (int32)GetItemData<UDWEquipAsset>()->PartType));
	}
}

void ADWAbilityEquip::SetVisible_Implementation(bool bVisible)
{
	GetRootComponent()->SetVisibility(bVisible, true);
}

void ADWAbilityEquip::OnAssemble_Implementation()
{
	
}

void ADWAbilityEquip::OnDischarge_Implementation()
{
	
}
