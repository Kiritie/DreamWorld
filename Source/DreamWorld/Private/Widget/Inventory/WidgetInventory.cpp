#include "Widget/Inventory/WidgetInventory.h"

UWidgetInventory::UWidgetInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UWidgetInventory::OnInitialize_Implementation(UObject* InOwner)
{
	Super::OnInitialize_Implementation(InOwner);
}

void UWidgetInventory::OnRefresh_Implementation()
{
	Super::OnRefresh_Implementation();
}
