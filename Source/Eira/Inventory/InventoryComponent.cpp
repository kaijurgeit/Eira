// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "InventoryItemDefinition.h"

UE_DISABLE_OPTIMIZATION


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

// FInventoryEntry& UInventoryComponent::GetExistingEntry(FInventoryEntry PickupEntry)
// {
// 	FInventoryEntry* InventoryEntry = Entries.FindByPredicate(
// 		[PickupEntry](const FInventoryEntry& Entry)
// 		{
// 			return Entry.ItemDef == PickupEntry.ItemDef;
// 		});
// 	
// 	if(!InventoryEntry)
// 	{
// 		
// 	}
// }

FInventoryEntry* UInventoryComponent::GetOrCreateEntry(UInventoryItemDefinition* PickupItem, EInventoryGroup Group)
{
	FInventoryEntry* InventoryEntry = Entries.FindByPredicate(
		[PickupItem](const FInventoryEntry& Entry)
		{
			return Entry.ItemDef->StaticClass() == PickupItem->StaticClass();
		});

	if(InventoryEntry)
	{
		return InventoryEntry;
	}
	
	if(FreeStacksPerGroup[Group] == 0)
	{
		return nullptr;
	}
	
	InventoryEntry = &Entries.AddDefaulted_GetRef();
	FreeStacksPerGroup[Group] -= 1;
	return InventoryEntry;
}

int32 UInventoryComponent::FreeStacksInGroup(FInventoryEntry PickupEntry, EInventoryGroup InventoryGroup)
{
	// Update Free Stacks
	int32 StacksOccupied = 0;
	for (const FInventoryEntry& InventoryEntry : Entries)
	{
		const UUInventoryFragment_InventoryEntryLayout* EntryLayout = Cast<UUInventoryFragment_InventoryEntryLayout>(
			PickupEntry.ItemDef->FindFragmentByClass(UUInventoryFragment_InventoryEntryLayout::StaticClass()));
		// Find Entries of the same group as the pickup e.g. Resources
		if(EntryLayout && EntryLayout->InventoryGroup == InventoryGroup)
		{
			int32 StacksOccupiedByItem = InventoryEntry.Count / EntryLayout->MaxStackCount + 1;
			StacksOccupied += StacksOccupiedByItem;
		}					
	}
	return StacksPerGroup[InventoryGroup] - StacksOccupied;
}

void UInventoryComponent::AddItemDefinition(FInventoryEntry PickupEntry)
{
	const UUInventoryFragment_InventoryEntryLayout* ItemLayout = Cast<UUInventoryFragment_InventoryEntryLayout>(
		PickupEntry.ItemDef->FindFragmentByClass(UUInventoryFragment_InventoryEntryLayout::StaticClass()));
	
	if(!ItemLayout)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("Inventory Full"));
		return;
	}

	const EInventoryGroup Group = ItemLayout->InventoryGroup;
	int32 FreeStacks = FreeStacksInGroup(PickupEntry, Group);

	// Get Entry if item definition already exists in inventory or create a new Entry
	FInventoryEntry* CurrentInventoryEntry = GetOrCreateEntry(PickupEntry.ItemDef, Group);

	// If there is no free stack left in the inventory group no Entry will be createds
	if(!CurrentInventoryEntry)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("Item could not be added because there is no free stack left in the inventory group"));
		return;
	}

	// How much space is free in the partially filled Stack?
	const int32 MaxStackCount = ItemLayout->MaxStackCount;
	const int32 MaxTotalCount = ItemLayout->MaxTotalCount;
	const int32 CurrentItemCount = CurrentInventoryEntry->Count;
	const int32 FreeSpaceInStack = MaxStackCount - (CurrentInventoryEntry->Count % MaxStackCount);	
	// How much space is left in total considering the partially filled Stack and all empty Stacks in the Group?
	int32 TotalCountFree = FreeStacksPerGroup[Group] * MaxStackCount + FreeSpaceInStack;
	TotalCountFree = (MaxTotalCount > 0) ? FMath::Min(TotalCountFree, MaxTotalCount) : TotalCountFree;

	// Add to Inventory
	const int32 OldItemCount = CurrentInventoryEntry->Count;
	const int32 TotalItemCount = CurrentInventoryEntry->Count + PickupEntry.Count;
	const int32 NewItemCount = (TotalItemCount > TotalCountFree) ? TotalCountFree : TotalItemCount;	
	CurrentInventoryEntry->Count = NewItemCount;

	// Update FreeStacksPerGroup
	const int32 NewStacksUsed = (NewItemCount / MaxStackCount) - (OldItemCount / MaxStackCount);
	FreeStacksPerGroup[Group] -= NewStacksUsed;

	// How many items did not fit into the inventory?
	const int32 RestCount = FMath::Max(0, TotalItemCount - TotalCountFree);
	
	UE_LOG(LogTemp, Warning, TEXT("%s -> Count: %i; Free Stacks: %i; Rest: %i"), *FString(__FUNCTION__), CurrentInventoryEntry->Count, FreeStacksPerGroup[Group], RestCount);
}


