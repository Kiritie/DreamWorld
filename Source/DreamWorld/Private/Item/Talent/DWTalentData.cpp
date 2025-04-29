#include "Item/Talent/DWTalentData.h"

#include "Character/DWCharacter.h"
#include "Widget/Talent/WidgetTalentItem.h"

UDWTalentData::UDWTalentData()
{
	MaxLevel = 5;

	TalentPoint = 1;
}

FString UDWTalentData::GetItemErrorInfo(FAbilityItem InItem) const
{
	FString ErrorInfo;
	if(UWidgetTalentItem* TalentItem = InItem.GetPayload<UWidgetTalentItem>())
	{
		if(TalentItem->GetParentItem().IsValid() && TalentItem->GetParentItem().Level < TalentItem->GetParentItem().GetData().MaxLevel)
		{
			ErrorInfo = TEXT("请先激活前置天赋！");
		}
	}
	return ErrorInfo;
}
