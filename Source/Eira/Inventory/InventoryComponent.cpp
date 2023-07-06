// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "InventoryItemDefinition.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItemDefinition(FInventoryEntry PickupInventoryEntry)
{
	for (FInventoryEntry& Entry : Entries)
	{
		if(Entry.ItemDef == PickupInventoryEntry.ItemDef)
		{
			Entry.StackCount += PickupInventoryEntry.StackCount;			
			UpdateInventory.Broadcast(Entry);
			return;
		}
	}
	Entries.Add(PickupInventoryEntry);
	UpdateInventory.Broadcast(PickupInventoryEntry);
}


