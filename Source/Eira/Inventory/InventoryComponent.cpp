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
	FreeStacksPerGroup = TMap<EInventoryGroup, int32>(MaxStacksPerGroup);
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
	InventoryEntry->ItemDef = PickupItem;
	FreeStacksPerGroup[Group] -= 1;
	return InventoryEntry;
}


int32 UInventoryComponent::AddItems(FInventoryEntry* Entry, const EInventoryGroup Group, const int32 MaxItemsPerStack, const int32 MaxItemsTotal, const int32 PickupItemCount)
{
	// How much space is left in total considering the partially filled Stack and all empty Stacks in the Group?
	const int32 FreeSpaceInStack = MaxItemsPerStack - (Entry->Count % MaxItemsPerStack);
	int32 FreeSpaceInTotal = FreeStacksPerGroup[Group] * MaxItemsPerStack + FreeSpaceInStack;
	FreeSpaceInTotal = ((FreeStacksPerGroup[Group] == 0) && (FreeSpaceInTotal == 10)) ? 0 : FreeSpaceInTotal;
	// MaxItemsTotal == 0 means that there is now limit regarding total item count and all free space can be used
	FreeSpaceInTotal = (MaxItemsTotal == 0) ? FreeSpaceInTotal : FMath::Min(FreeSpaceInTotal, MaxItemsTotal);

	// Add to Inventory
	const int32 OldCount = Entry->Count; 
	const int32 NewItemCount = (PickupItemCount > FreeSpaceInTotal) ? OldCount + FreeSpaceInTotal : OldCount + PickupItemCount;
	Entry->Count = NewItemCount;

	// Update FreeStacksPerGroup
	const int32 NewStacksUsed = ((NewItemCount - 1) / MaxItemsPerStack) - ((OldCount - 1) / MaxItemsPerStack);
	FreeStacksPerGroup[Group] -= NewStacksUsed;

	const int32 ItemsAdded = NewItemCount - OldCount;
	return ItemsAdded;
}

void UInventoryComponent::AddItemDefinition(FInventoryEntry& PickupEntry)
{
	// Get Item Layout and Group
	const UUInventoryFragment_InventoryEntryLayout* Layout = Cast<UUInventoryFragment_InventoryEntryLayout>(
		PickupEntry.ItemDef->FindFragmentByClass(UUInventoryFragment_InventoryEntryLayout::StaticClass()));	
	if(!Layout)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("Inventory Full"));
		return;
	}

	// Get Entry if item definition already exists in inventory or create a new Entry
	FInventoryEntry* Entry = GetOrCreateEntry(PickupEntry.ItemDef, Layout->Group);

	// If there is no free stack left in the inventory group no Entry will be created
	if(!Entry)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("Item could not be added because there is no free stack left in the inventory group"));
		return;
	}

	const int32 ItemsAdded = AddItems(Entry, Layout->Group, Layout->MaxItemsPerStack, Layout->MaxItemsTotal, PickupEntry.Count);
	
	// How many items did not fit into the inventory?
	PickupEntry.Count -= ItemsAdded;
	
	UE_LOG(LogTemp, Warning, TEXT("%s -> Count: %i; Free Stacks: %i; Rest: %i"), *FString(__FUNCTION__), Entry->Count, FreeStacksPerGroup[Layout->Group], PickupEntry.Count);
}


