// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"
#include "GameFramework/Actor.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryComponent.h"

class UActorComponent;

TScriptInterface<IPickupable> UPickupableStatics::GetFirstPickupableFromActor(AActor* Actor)
{
	// If the actor is directly pickupable, return that.
	TScriptInterface<IPickupable> PickupableActor(Actor);
	if (PickupableActor)
	{
		return PickupableActor;
	}

	// If the actor isn't pickupable, it might have a component that has a pickupable interface.
	TArray<UActorComponent*> PickupableComponents = Actor ? Actor->GetComponentsByInterface(UPickupable::StaticClass()) : TArray<UActorComponent*>();
	if (PickupableComponents.Num() > 0)
	{
		// Get first pickupable, if the user needs more sophisticated pickup distinction, will need to be solved elsewhere.
		return TScriptInterface<IPickupable>(PickupableComponents[0]);
	}

	return TScriptInterface<IPickupable>();
}

// Add default functionality here for any IPickupable functions that are not pure virtual.
void UPickupableStatics::AddPickupToInventory(UInventoryComponent* InventoryComponent, TScriptInterface<IPickupable> Pickup)
{
	if(InventoryComponent && Pickup)
	{
		TArray<FInventoryClassEntry>& PickupInventoryClassEntries = Pickup->GetPickupInventory();
		// TArray<FInventoryEntry> PickupInventoryEntries = Pickup->GetPickupInventory();
		for (FInventoryClassEntry& PickupClassEntry : PickupInventoryClassEntries)
		{
			const int32 ItemsAdded = InventoryComponent->AddItemDefinition(PickupClassEntry.ItemDef, PickupClassEntry.Count);
			PickupClassEntry.Count -= ItemsAdded;
		}
	}
}
