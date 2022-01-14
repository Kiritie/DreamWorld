// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WidgetGameHUD.h"

#include "Character/DWCharacter.h"
#include "Interaction/Components/InteractionComponent.h"
#include "Main/MainModule.h"
#include "Widget/WidgetModule.h"


UWidgetGameHUD::UWidgetGameHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WidgetName = FName("GameHUD");
	WidgetType = EWidgetType::SemiPermanent;
	InputMode = EInputMode::GameOnly;
}

void UWidgetGameHUD::OnCreate_Implementation()
{
	Super::OnCreate_Implementation();
	
	if(AInputModule* InputModule = AMainModule::GetModuleByClass<AInputModule>())
	{
		InputModule->OnChangeInputMode.AddDynamic(this, &UWidgetGameHUD::OnChangeInputMode);
	}
}

void UWidgetGameHUD::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetGameHUD::OnOpen_Implementation(const TArray<FParameter>& InParams, bool bInstant)
{
	Super::OnOpen_Implementation(InParams, bInstant);

	FinishOpen(bInstant);
}

void UWidgetGameHUD::OnClose_Implementation(bool bInstant)
{
	Super::OnClose_Implementation(bInstant);

	FinishClose(bInstant);
}

void UWidgetGameHUD::RefreshActions()
{
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(GetOwnerActor()))
	{
		if(IInteraction* OverlappingTarget = OwnerCharacter->GetInteractionComponent()->GetOverlappingTarget())
		{
			ShowActions(OverlappingTarget->GetInteractionComponent()->GetValidInteractActions(OwnerCharacter));
		}
		else
		{
			HideActions();
		}
	}
}

void UWidgetGameHUD::OnChangeInputMode(EInputMode InInputMode)
{
	if(ADWCharacter* OwnerCharacter = Cast<ADWCharacter>(OwnerActor))
	{
		if(!OwnerCharacter->IsDead() && InInputMode == EInputMode::GameOnly)
		{
			SetCrosshairVisible(true);
		}
		else
		{
			SetCrosshairVisible(true);
		}
	}
	else
	{
		SetCrosshairVisible(true);
	}
}
