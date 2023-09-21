// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/DWEquip.h"

#include "Ability/Character/AbilityCharacterBase.h"
#include "Item/Equip/DWEquipData.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Common/CommonBPLibrary.h"
#include "Ability/Inventory/AbilityInventoryBase.h"

ADWEquip::ADWEquip()
{

}

void ADWEquip::Initialize_Implementation(AAbilityCharacterBase* InOwnerCharacter, const FAbilityItem& InItem)
{
	Super::Initialize_Implementation(InOwnerCharacter, InItem);
}

void ADWEquip::OnAssemble_Implementation()
{
	Super::OnAssemble_Implementation();

	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, *UCommonBPLibrary::GetEnumValueAuthoredName(TEXT("/Script/DreamWorld.EDWEquipPartType"), (int32)GetItemData<UDWEquipData>().PartType));
}

void ADWEquip::OnDischarge_Implementation()
{
	Super::OnDischarge_Implementation();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
