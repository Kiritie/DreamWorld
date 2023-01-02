#include "Vitality/DWVitalityData.h"

#include "Ability/AbilityModuleBPLibrary.h"

UDWVitalityData::UDWVitalityData()
{
	// tags
	DeadTag = FGameplayTag::RequestGameplayTag("State.Vitality.Dead");
	DyingTag = FGameplayTag::RequestGameplayTag("State.Vitality.Dying");
}
