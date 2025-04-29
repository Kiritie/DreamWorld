#include "Item/Skill/DWSkillData.h"

#include "Ability/Inventory/AbilityInventoryBase.h"
#include "Ability/Inventory/Slot/AbilityInventorySlotBase.h"
#include "Character/DWCharacter.h"
#include "Common/CommonStatics.h"

UDWSkillData::UDWSkillData()
{
	MaxLevel = 100;

	SkillType = EDWSkillType::None;
	WeaponPart = EDWWeaponPart::None;
	WeaponType = EDWWeaponType::None;
	bCancelAble = false;
}

FString UDWSkillData::GetItemErrorInfo(FAbilityItem InItem) const
{
	FString ErrorInfo = Super::GetItemErrorInfo(InItem);
	if(UAbilityInventorySlotBase* InventorySlot = InItem.GetPayload<UAbilityInventorySlotBase>())
	{
		if(ADWCharacter* Character = InventorySlot->GetInventory()->GetOwnerAgent<ADWCharacter>())
		{
			if(!Character->CheckWeaponType(WeaponPart, WeaponType))
			{
				ErrorInfo.Appendf(TEXT("\n角色未装备[%s]"), *UCommonStatics::GetEnumDisplayNameByValue(TEXT("/Script/DreamWorld.EDWWeaponType"), (int32)WeaponType).ToString());
			}
			ErrorInfo.RemoveFromStart(TEXT("\n"));
		}
	}
	return ErrorInfo;
}
