#include "Widget/Inventory/WidgetInventory.h"

#include "Components/GridPanel.h"
#include "Inventory/Slot/InventorySkillSlot.h"
#include "Inventory/Slot/InventorySlot.h"
#include "Vitality/Vitality.h"

UWidgetInventory::UWidgetInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWidgetInventory::OnInitialize_Implementation(AActor* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
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
	if(UISlotDatas.Contains(InSplitSlotType))
	{
		return UISlotDatas[InSplitSlotType].Slots;
	}
	return TArray<UWidgetInventorySlot*>();
}
