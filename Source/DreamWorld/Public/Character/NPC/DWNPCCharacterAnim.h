// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Human/DWHumanCharacterAnim.h"
#include "DWNPCCharacterAnim.generated.h"

class ADWCharacter;

/**
 * ���˽�ɫ����������
 */
UCLASS()
class DREAMWORLD_API UDWNPCCharacterAnim : public UDWHumanCharacterAnim
{
	GENERATED_BODY()

public:
	UDWNPCCharacterAnim();
	
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
