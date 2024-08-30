// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Interaction/WidgetInteractionBox.h"

#include "Ability/PickUp/AbilityPickUpBase.h"
#include "Character/DWCharacter.h"
#include "Components/HorizontalBox.h"
#include "Voxel/Voxels/Auxiliary/VoxelInteractAuxiliary.h"
#include "Widget/WidgetModule.h"
#include "Widget/Item/WidgetAbilityPreviewItem.h"

UWidgetInteractionBox::UWidgetInteractionBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("InteractionBox");
	ParentName = FName("GameHUD");
	ParentSlot = FName("Slot_InteractionBox");
	WidgetType = EWidgetType::Permanent;
	WidgetCreateType = EWidgetCreateType::AutoCreateAndOpen;
	
	WBP_PreviewItem = nullptr;
	HBox_NextKeyTips = nullptr;
}

void UWidgetInteractionBox::OnCreate(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnCreate(InOwner, InParams);

	HBox_NextKeyTips->VisibilityDelegate.BindDynamic(this, &UWidgetInteractionBox::GetNextKeyTipsVisibility);
}

void UWidgetInteractionBox::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetInteractionBox::OnOpen(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen(InParams, bInstant);
}

void UWidgetInteractionBox::OnRefresh()
{
	Super::OnRefresh();
}

void UWidgetInteractionBox::OnClose(bool bInstant)
{
	Super::OnClose(bInstant);
}

void UWidgetInteractionBox::ShowInteractActions_Implementation(const TScriptInterface<IInteractionAgentInterface>& InInteractionAgent, const TArray<EInteractAction>& InActions)
{
	if(!InInteractionAgent || InActions.IsEmpty()) return;

	InteractionAgent = InInteractionAgent;

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
		Item = FAbilityItem(Entity->GetAssetID());
	}
	
	if(Item.IsValid())
	{
		WBP_PreviewItem->Init({ &Item });
		WBP_PreviewItem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWidgetInteractionBox::HideInteractActions_Implementation()
{
	InteractionAgent = nullptr;

	WBP_PreviewItem->SetVisibility(ESlateVisibility::Collapsed);
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
