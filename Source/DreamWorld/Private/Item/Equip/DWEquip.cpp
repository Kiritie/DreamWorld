// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/DWEquip.h"

#include "Item/Equip/DWEquipData.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Common/CommonStatics.h"
#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Character/DWCharacter.h"

ADWEquip::ADWEquip()
{

}

void ADWEquip::Initialize_Implementation(AActor* InOwnerActor, const FAbilityItem& InItem)
{
	Super::Initialize_Implementation(InOwnerActor, InItem);
}

void ADWEquip::OnAssemble_Implementation()
{
	Super::OnAssemble_Implementation();

	AttachToComponent(GetOwnerActor<ADWCharacter>()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, *UCommonStatics::GetEnumValueAuthoredName(TEXT("/Script/DreamWorld.EDWEquipPartType"), (int32)GetItemData<UDWEquipData>().PartType));
}

void ADWEquip::OnDischarge_Implementation()
{
	Super::OnDischarge_Implementation();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
