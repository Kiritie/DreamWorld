// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ability/Character/AbilityCharacterAnimBase.h"
#include "DWCharacterAnim.generated.h"

/**
 * 角色动画控制器
 */
UCLASS()
class DREAMWORLD_API UDWCharacterAnim : public UAbilityCharacterAnimBase
{
	GENERATED_BODY()

public:
	UDWCharacterAnim();

protected:
	virtual void NativeHandleNotify(const FString& AnimNotifyName) override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDefending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRiding;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bClimbing;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAiming;
};
