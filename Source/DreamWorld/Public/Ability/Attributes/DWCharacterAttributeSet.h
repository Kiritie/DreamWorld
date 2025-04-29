#pragma once

#include "Ability/Attributes/CharacterAttributeSetBase.h"
#include "DWCharacterAttributeSet.generated.h"

/**
 * 角色属性集
 */
UCLASS()
class DREAMWORLD_API UDWCharacterAttributeSet : public UCharacterAttributeSetBase
{
	GENERATED_BODY()

public:
	UDWCharacterAttributeSet();

public:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData Mana;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Mana)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData MaxMana;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxMana)
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData ManaRecovery;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ManaRecovery)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData ManaRegenSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ManaRegenSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData Stamina;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Stamina)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData MaxStamina;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MaxStamina)
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData StaminaRecovery;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaRecovery)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData StaminaRegenSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaRegenSpeed)
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData StaminaExpendSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, StaminaExpendSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData Hunger;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Hunger)
				
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData HungerRecovery;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, HungerRecovery)
				
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData HungerExpendSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, HungerExpendSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData Thirst;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Thirst)
								
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData ThirstRecovery;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ThirstRecovery)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData ThirstExpendSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ThirstExpendSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData Oxygen;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, Oxygen)
					
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData OxygenRecovery;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, OxygenRecovery)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData OxygenRegenSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, OxygenRegenSpeed)
			
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData OxygenExpendSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, OxygenExpendSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData DodgeForce;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DodgeForce)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData RepulseForce;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, RepulseForce)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData AttackForce;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackForce)
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData AttackSpeed;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackSpeed)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData AttackCritRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackCritRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData AttackStealRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, AttackStealRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData DefendRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData DefendScopeRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, DefendScopeRate)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData PhysicsRes;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, PhysicsRes)

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData MagicRes;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, MagicRes)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "CharacterAttributes")
	FGameplayAttributeData ToughnessRate;
	GAMEPLAYATTRIBUTE_ACCESSORS(UDWCharacterAttributeSet, ToughnessRate)
};
