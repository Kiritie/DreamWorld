// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Components/WidgetVitalityHPComponent.h"

#include "Ability/Vitality/AbilityVitalityBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Global/GlobalBPLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Widget/World/WidgetVitalityHP.h"

UWidgetVitalityHPComponent::UWidgetVitalityHPComponent()
{
	static ConstructorHelpers::FClassFinder<UWidgetVitalityHP> VitalityHPClassFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widget/World/WB_VitalityHP.WB_VitalityHP_C'"));
	if(VitalityHPClassFinder.Succeeded())
	{
		SetWidgetClass(VitalityHPClassFinder.Class);
	}
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2D(220, 60));
	SetPivot(FVector2D(0.5f, 1));
}

void UWidgetVitalityHPComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerVitality = Cast<AAbilityVitalityBase>(GetOwner());
}

void UWidgetVitalityHPComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!OwnerVitality) return;

	RefreshVisibility();
}

void UWidgetVitalityHPComponent::RefreshVisibility() const
{
	if(GetUserWidgetObject())
	{
		ADWPlayerCharacter* PlayerCharacter = UGlobalBPLibrary::GetPlayerCharacter<ADWPlayerCharacter>();
		if(!OwnerVitality->IsDead() && PlayerCharacter && FVector::Distance(OwnerVitality->GetActorLocation(), PlayerCharacter->GetActorLocation()) < 1000)
		{
			GetUserWidgetObject()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			GetUserWidgetObject()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
