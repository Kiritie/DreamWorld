// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equip/DWEquip.h"

#include "Item/Equip/DWEquipData.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Common/CommonStatics.h"
#include "Character/DWCharacter.h"

ADWEquip::ADWEquip()
{

}

void ADWEquip::OnAssemble_Implementation()
{
	Super::OnAssemble_Implementation();

	ADWCharacter* Character = GetOwnerActor<ADWCharacter>();

	Character->AttachActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, *UCommonStatics::GetEnumAuthoredNameByValue(TEXT("/Script/DreamWorld.EDWEquipSocket"), (int32)GetItemData<UDWEquipData>().EquipSocket));
}

void ADWEquip::OnDisassemble_Implementation()
{
	Super::OnDisassemble_Implementation();

	ADWCharacter* Character = GetOwnerActor<ADWCharacter>();
	
	Character->DetachActor(this, FDetachmentTransformRules::KeepWorldTransform);
}
