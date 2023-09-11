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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bDefending;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bRiding;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStates")
	bool bClimbing;

protected:
	virtual void NativeHandleNotify(const FString& AnimNotifyName) override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
