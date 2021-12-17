// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionComponent.generated.h"

class IInteraction;
/**
 * 
 */
UCLASS()
class DREAMWORLD_API UInteractionComponent : public UBoxComponent
{
	GENERATED_BODY()
		
public:
	UInteractionComponent(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EInteractAction> InteractActions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EInteractAction InteractingAction;
	
	IInteraction* OverlappingTarget;

	IInteraction* InteractionTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual bool DoInteract(IInteraction* InInteractionTarget, EInteractAction InInteractAction);

public:
	void AddInteractionAction(EInteractAction InInteractAction);
		
	void RemoveInteractionAction(EInteractAction InInteractAction);
		
	TArray<EInteractAction> GetValidInteractActions(IInteraction* InInteractionTarget) const;
	
	EInteractAction GetInteractingAction() const { return InteractingAction; }

	IInteraction* GetOverlappingTarget() const { return OverlappingTarget; }

	IInteraction* GetInteractionOwner() const;

	IInteraction* GetInteractionTarget() const { return InteractionTarget; }
	
	void SetInteractionTarget(IInteraction* InInteractionTarget);
};
