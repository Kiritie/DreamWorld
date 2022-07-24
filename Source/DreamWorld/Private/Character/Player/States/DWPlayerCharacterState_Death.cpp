// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/States/DWPlayerCharacterState_Death.h"
#include "Ability/Item/Equip/AbilityEquipBase.h"
#include "AI/DWAIController.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/Inventory.h"
#include "Voxel/Chunks/VoxelChunk.h"
#include "Widget/WidgetGameHUD.h"
#include "Widget/WidgetModuleBPLibrary.h"

UDWPlayerCharacterState_Death::UDWPlayerCharacterState_Death()
{
	
}

void UDWPlayerCharacterState_Death::OnInitialize(UFSMComponent* InFSMComponent, int32 InStateIndex)
{
	Super::OnInitialize(InFSMComponent, InStateIndex);
}

bool UDWPlayerCharacterState_Death::OnEnterValidate(UFiniteStateBase* InLastFiniteState)
{
	return Super::OnEnterValidate(InLastFiniteState);
}

void UDWPlayerCharacterState_Death::OnEnter(UFiniteStateBase* InLastFiniteState)
{
	Super::OnEnter(InLastFiniteState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	if(PlayerCharacter->GetTargetSystem()->IsLocked())
	{
		PlayerCharacter->GetTargetSystem()->TargetLockOff();
	}
}

void UDWPlayerCharacterState_Death::OnRefresh()
{
	Super::OnRefresh();

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

}

void UDWPlayerCharacterState_Death::OnLeave(UFiniteStateBase* InNextFiniteState)
{
	Super::OnLeave(InNextFiniteState);

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

}

void UDWPlayerCharacterState_Death::OnTermination()
{
	Super::OnTermination();
}

void UDWPlayerCharacterState_Death::DeathStart()
{
	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

}

void UDWPlayerCharacterState_Death::DeathEnd()
{
	Super::DeathEnd();

	ADWPlayerCharacter* PlayerCharacter = GetAgent<ADWPlayerCharacter>();

	if(UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>())
	{
		UWidgetModuleBPLibrary::GetUserWidget<UWidgetGameHUD>()->SetCrosshairVisible(false);
	}
}
