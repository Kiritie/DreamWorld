// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld.h"
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
	virtual void UpdateAnimParams(float DeltaSeconds) override;
};
