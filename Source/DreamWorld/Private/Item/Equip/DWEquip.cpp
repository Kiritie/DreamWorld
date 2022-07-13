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

ADWEquip::ADWEquip()
{

}

void ADWEquip::Initialize(AAbilityCharacterBase* InOwnerCharacter)
{
	Super::Initialize(InOwnerCharacter);
	if (const ADWCharacter* Character = Cast<ADWCharacter>(InOwnerCharacter))
	{
		Character->GetInventory()->GetSplitSlots<UInventoryEquipSlot>(ESplitSlotType::Equip)[(int32)GetItemData<UDWEquipData>().PartType]->SetPartType(GetItemData<UDWEquipData>().PartType);
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, *UGlobalBPLibrary::GetEnumValueAuthoredName(TEXT("EDWEquipPartType"), (int32)GetItemData<UDWEquipData>().PartType));
	}
}

void ADWEquip::OnAssemble_Implementation()
{
	Super::OnAssemble_Implementation();
}

void ADWEquip::OnDischarge_Implementation()
{
	Super::OnDischarge_Implementation();
}
