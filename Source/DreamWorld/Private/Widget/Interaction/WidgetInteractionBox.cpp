// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Interaction/WidgetInteractionBox.h"

#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Character/DWCharacter.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Input/Widget/WidgetKeyTipsItemBase.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/WidgetModule.h"
#include "Widget/Item/WidgetAbilityItem.h"

UWidgetInteractionBox::UWidgetInteractionBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InteractionBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_InteractionBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreate;
	
	PreviewItem = nullptr;
	NextKeyTips = nullptr;
	
	InteractionAgent = nullptr;
}

void UWidgetInteractionBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	NextKeyTips->VisibilityDelegate.BindDynamic(this, &UWidgetInteractionBox::GetNextKeyTipsVisibility);
}

void UWidgetInteractionBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetInteractionBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	if(InParams.IsValidIndex(0))
	{
		InteractionAgent = InParams[0];
	}

	Super::OnOpen(InParams, bInstant);

	FAbilityItem Item;
	
	if(AAbilityPickUpBase* PickUp = Cast<AAbilityPickUpBase>(InteractionAgent.GetObject()))
	{
		Item = PickUp->GetItem();
	}
	else if(AVoxelInteractAuxiliary* Auxiliary = Cast<AVoxelInteractAuxiliary>(InteractionAgent.GetObject()))
	{
		Item = FAbilityItem(Auxiliary->GetVoxelItem());
	}
	else if(IPrimaryEntityInterface* Entity = Cast<IPrimaryEntityInterface>(InteractionAgent.GetObject()))
	{
		Item = FAbilityItem(Entity->Execute_GetAssetID(InteractionAgent.GetObject()));
	}
	
	if(Item.IsValid() && InteractionAgent != GetOwnerObject())
	{
		PreviewItem->Init({ &Item });
		PreviewItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetInteractionBox::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetInteractionBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);

	InteractionAgent = nullptr;
}

TArray<EInteractAction> UWidgetInteractionBox::GetInteractActions() const
{
	if(ADWPlayerCharacter* Character = GetOwnerObject<ADWPlayerCharacter>())
	{
		return Character->GetInteractableActions(InteractionAgent.GetInterface());
	}
	return TArray<EInteractAction>();
}

ESlateVisibility UWidgetInteractionBox::GetNextKeyTipsVisibility()
{
	if(IInteractionAgentInterface* OwnerInteractionAgent = GetOwnerObject<IInteractionAgentInterface>())
	{
		if(InteractionAgent && OwnerInteractionAgent->GetOverlappingAgents().Num() > 1)
		{
			return ESlateVisibility::SelfHitTestInvisible;
		}
	}
	return ESlateVisibility::Collapsed;
}
