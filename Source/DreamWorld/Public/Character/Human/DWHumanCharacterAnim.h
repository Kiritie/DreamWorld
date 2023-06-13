// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/DWCharacterAnim.h"
#include "DWHumanCharacterAnim.generated.h"

class ADWCharacter;

/**
 * ���˽�ɫ����������
 */
UCLASS()
class DREAMWORLD_API UDWHumanCharacterAnim : public UDWCharacterAnim
{
	GENERATED_BODY()

public:
	UDWHumanCharacterAnim();
	
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
