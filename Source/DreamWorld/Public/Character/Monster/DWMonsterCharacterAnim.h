// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DreamWorld.h"
#include "Character/DWCharacterAnim.h"
#include "DWMonsterCharacterAnim.generated.h"

class ADWCharacter;

/**
 * �����ɫ����������
 */
UCLASS()
class DREAMWORLD_API UDWMonsterCharacterAnim : public UDWCharacterAnim
{
	GENERATED_BODY()

public:
	UDWMonsterCharacterAnim();

protected:
	virtual void UpdateAnimParams(float DeltaSeconds) override;
};
