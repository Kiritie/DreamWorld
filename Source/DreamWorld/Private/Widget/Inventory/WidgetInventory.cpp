#include "Widget/Inventory/WidgetInventory.h"

#include "Inventory/Slot/InventorySlot.h"
#include "Vitality/Vitality.h"
#include "Widget/Inventory/Slot/WidgetInventorySlot.h"

UWidgetInventory::UWidgetInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWidgetInventory::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}

UInventory* UWidgetInventory::GetInventory() const
{
	if(IVitality* Vitality = Cast<IVitality>(OwnerActor))
	{
		return Vitality->GetInventory();
	}
	return nullptr;
}

TArray<UWidgetInventorySlot*> UWidgetInventory::GetSplitUISlots(ESplitSlotType InSplitSlotType) const
{
	TArray<UWidgetInventorySlot*> ReturnValue;
	for (auto Iter : UISlots)
	{
		if(Iter->GetOwnerSlot()->GetSplitType() == InSplitSlotType)
		{
			ReturnValue.Add(Iter);
		}
	}
	return ReturnValue;
}
