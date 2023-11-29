#include "Widget/Inventory/WidgetInventory.h"

UWidgetInventory::UWidgetInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWidgetInventory::OnInitialize(UObject* InOwner, const TArray<FParameter>& InParams)
{
	Super::OnInitialize(InOwner, InParams);
}

void UWidgetInventory::OnRefresh()
{
	Super::OnRefresh();
}
