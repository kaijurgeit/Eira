// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "InventoryItemDefinition.h"
#include "ItemFragments/InventoryFragment_AttachableItem.h"
#include "ItemFragments/InventoryFragment_EquippableItem.h"
#include "Items/Item.h"
#include "Player/EiraCharacter.h"

// UE_DISABLE_OPTIMIZATION

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
	EiraCharacterOwner = Cast<AEiraCharacter>(GetOwner());

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

int32 UInventoryComponent::AddItemDefinition(const FInventoryEntry& PickupEntry)
{
	const UInventoryFragment_AttachableItem* Attachable = Cast<UInventoryFragment_AttachableItem>(
		PickupEntry.ItemDef->FindFragmentByClass(UInventoryFragment_AttachableItem::StaticClass()));	
	if(Attachable)
	{
		AItem* Item = GetWorld()->SpawnActor<AItem>(Attachable->ItemClass, FTransform());
		GetEiraCharacterOwner()->AttachToSocket(Item, Attachable->AttachSocket);
	}
	
	// Get ItemClass Layout and Group
	const UInventoryFragment_InventoryEntryLayout* Layout = Cast<UInventoryFragment_InventoryEntryLayout>(
		PickupEntry.ItemDef->FindFragmentByClass(UInventoryFragment_InventoryEntryLayout::StaticClass()));	
	if(!Layout)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("No Entry Layout"));
		return 0;
	}

	// Get Entry if item definition already exists in inventory or create a new Entry
	FInventoryEntry* Entry = GetOrCreateEntry(PickupEntry.ItemDef, Layout->Group);

	// If there is no free stack left in the inventory group no Entry will be created
	if(!Entry)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("ItemClass could not be added because there is no free stack left in the inventory group"));
		return 0;
	}

	const int32 ItemsAdded = AddItems(Entry, Layout->Group, Layout->MaxItemsPerStack, Layout->MaxItemsTotal, PickupEntry.Count);
	UpdateInventory.Broadcast(Entries);
	
	return ItemsAdded;
}

int32 UInventoryComponent::RemoveItemDefinition(const FInventoryEntry& DropEntry)
{	
	const UInventoryFragment_InventoryEntryLayout* Layout = Cast<UInventoryFragment_InventoryEntryLayout>(
		DropEntry.ItemDef->FindFragmentByClass(UInventoryFragment_InventoryEntryLayout::StaticClass()));
	if(!Layout)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("No Entry Layout"));
		return 0;
	}
	
	int32 ItemsRemoved = 0;
	const int Index = Entries.FindLastByPredicate(
		[DropEntry](const FInventoryEntry& Entry)
		{
			return Entry.ItemDef->StaticClass() == DropEntry.ItemDef->StaticClass();
		});
	if(Index == INDEX_NONE) { return ItemsRemoved; }
	
	ItemsRemoved = FMath::Min(DropEntry.Count, Entries[Index].Count);
	Entries[Index].Count -= ItemsRemoved;

	if(Entries[Index].Count == 0)
	{
		Entries.RemoveAt(Index);
		FreeStacksPerGroup[Layout->Group] += 1;
	}
	UpdateInventory.Broadcast(Entries);

	return ItemsRemoved;	
}

void UInventoryComponent::Select(const UInventoryItemDefinition* ItemDef)
{
	const UInventoryFragment_EquippableItem* Equippable = Cast<UInventoryFragment_EquippableItem>(
		ItemDef->FindFragmentByClass(UInventoryFragment_EquippableItem::StaticClass()));

	if(Equippable)
	{
		AItem* Item = GetWorld()->SpawnActor<AItem>(Equippable->ItemClass, FTransform());
		GetEiraCharacterOwner()->Equip(Item, Equippable->AttachSocket);
	}
}

AEiraCharacter* UInventoryComponent::GetEiraCharacterOwner()
{
	return EiraCharacterOwner;
}


