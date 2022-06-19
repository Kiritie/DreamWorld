// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/DWEquip.h"

#include "Ability/Character/AbilityCharacterBase.h"
#include "Item/Equip/DWEquipData.h"
#include "Ability/Item/Equip/AbilityEquipDataBase.h"
#include "Character/DWCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Global/GlobalBPLibrary.h"
#include "Inventory/Inventory.h"
#include "Inventory/Slot/InventoryEquipSlot.h"

// Sets default values
ADWEquip::ADWEquip()
{

}

// Called when the game starts or when spawned
void ADWEquip::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADWEquip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADWEquip::Initialize(AAbilityCharacterBase* InOwnerCharacter)
{
	Super::Initialize(InOwnerCharacter);
	if (GetOwnerCharacter<ADWCharacter>())
	{
		GetOwnerCharacter<ADWCharacter>()->GetInventory()->GetSplitSlots<UInventoryEquipSlot>(ESplitSlotType::Equip)[(int32)GetItemData<UDWEquipData>().PartType];
		AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, *UGlobalBPLibrary::GetEnumValueAuthoredName(TEXT("EEquipPartType"), (int32)GetItemData<UDWEquipData>().PartType));
	}
}

void ADWEquip::SetVisible_Implementation(bool bVisible)
{
	GetRootComponent()->SetVisibility(bVisible, true);
}

void ADWEquip::OnAssemble_Implementation()
{
	
}

void ADWEquip::OnDischarge_Implementation()
{
	
}
