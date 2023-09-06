// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "InventoryItemDefinition.h"
#include "ItemFragments/InventoryFragment_AttachableItem.h"
#include "ItemFragments/InventoryFragment_EquippableItem.h"
#include "ItemFragments/InventoryItemFragment_SetGameplayAttributes.h"
#include "Items/Item.h"
#include "Player/EiraCharacter.h"

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

void UInventoryComponent::TrySetGameplayAttributes(UInventoryItemDefinition* ItemDef, const int32 ItemCount)
{
	if(const auto* SetAttributeFragment = ItemDef->FindFragmentByClass<UInventoryItemFragment_SetGameplayAttributes>())
	{
		check(GESetAnyAttributeByCaller);
		UAbilitySystemComponent* ASC = GetEiraCharacterOwner()->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GESetAnyAttributeByCaller, 1, EffectContextHandle);
		
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

		if(SetAttributeFragment->Attribute.IsValid())
		{
			Spec->SetSetByCallerMagnitude(SetAttributeFragment->Attribute, ItemCount);
		}

		for (auto Attribute : SetAttributeFragment->AttributeMap)
		{		
			Spec->SetSetByCallerMagnitude(Attribute.Key, Attribute.Value);
		}
		
		ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}

int32 UInventoryComponent::AddItemDefinition(TSubclassOf<UInventoryItemDefinition> ItemDefClass, int32 Count)
{
	// Create Instance from TSubclassOf<UInventoryItemDefinition> with Subclass as type
	UInventoryItemDefinition* ItemDef = NewObject<UInventoryItemDefinition>(this, ItemDefClass, NAME_None, RF_NoFlags, ItemDefClass->GetDefaultObject(), true);
	ItemDef->Fragments = GetDefault<UInventoryItemDefinition>(ItemDefClass)->Fragments;
	
	// Get ItemClass Layout and Group
	const auto* Layout = ItemDef->FindFragmentByClass<UInventoryFragment_InventoryMenu>();
	if(!Layout)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("No Entry Layout"));
		return 0;
	}

	// Get Entry if item definition already exists in inventory or create a new Entry
	FInventoryEntry* Entry = GetOrCreateEntry(ItemDef, Layout->Group);

	// If there is no free stack left in the inventory group no Entry will be created
	if(!Entry)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *FString(__FUNCTION__), *FString("ItemClass could not be added because there is no free stack left in the inventory group"));
		return 0;
	}

	const int32 ItemsAdded = AddItems(Entry, Layout->Group, Layout->MaxItemsPerStack, Layout->MaxItemsTotal, Count);
	if(ItemsAdded == 0)
	{
		return 0;
	}

	TrySetGameplayAttributes(ItemDef, ItemsAdded);
	
	TryAttachItem(ItemDef);
	
	UpdateInventory.Broadcast(Entries);
	
	return ItemsAdded;
}


int32 UInventoryComponent::RemoveItemDefinition(UInventoryItemDefinition* ItemDef, int32 Count)
{	
	const auto* Layout = ItemDef->FindFragmentByClass<UInventoryFragment_InventoryMenu>();
	if(!Layout)
	{
		UE_LOG(LogTemp, Warning, TEXT(	"%s -> %s"), *FString(__FUNCTION__), *FString("No Entry Layout"));
		return 0;
	}
	
	const int Index = Entries.FindLastByPredicate(
		[ItemDef](const FInventoryEntry& Entry)
		{
			return Entry.ItemDef->IsA(ItemDef->GetClass());
		});
	if(Index == INDEX_NONE) { return 0; }

	const int32 ItemsRemoved = RemoveItems(Layout->Group, Layout->MaxItemsPerStack, Count, Index);

	TrySetGameplayAttributes(ItemDef, -ItemsRemoved);
	
	UpdateInventory.Broadcast(Entries);

	return ItemsRemoved;
}

void UInventoryComponent::Select(const UInventoryItemDefinition* ItemDef)
{
	if(!ItemDef) { return; }
	
	const auto* Equippable = ItemDef->FindFragmentByClass<UInventoryFragment_EquippableItem>();

	if(!Equippable) { return; }

	if(Equippable->ItemClass)
	{		
		AItem* Item = GetWorld()->SpawnActor<AItem>(Equippable->ItemClass, FTransform());
		GetEiraCharacterOwner()->Equip(Item, Equippable->AttachSocket);		
	}

	const auto* Attachable = ItemDef->FindFragmentByClass<UInventoryFragment_AttachableItem>();

	if(Attachable)
	{
		GetEiraCharacterOwner()->ClearSocket(Attachable->SocketName);
	}
}

AEiraCharacter* UInventoryComponent::GetEiraCharacterOwner()
{
	return EiraCharacterOwner;
}

void UInventoryComponent::UpdateFreeStacks(const EInventoryGroup Group, const int32 MaxItemsPerStack, const int32 OldCount, const int32 NewCount)
{
	const int32 NewStacksUsed = (NewCount / MaxItemsPerStack) - (OldCount / MaxItemsPerStack);
	FreeStacksPerGroup[Group] -= NewStacksUsed;
	FreeStacksPerGroup[Group] = FMath::Clamp(FreeStacksPerGroup[Group], 0, MaxStacksPerGroup[Group]);
}

int32 UInventoryComponent::AddItems(FInventoryEntry* Entry, const EInventoryGroup Group, const int32 MaxItemsPerStack, const int32 MaxItemsTotal, const int32 PickupItemCount)
{
	// How much space is left in total considering the partially filled Stack and all empty Stacks in the Group?
	const int32 FreeSpaceInStack = MaxItemsPerStack - (Entry->Count % MaxItemsPerStack);
	int32 FreeSpaceInTotal = (FreeStacksPerGroup[Group] - 1) * MaxItemsPerStack + FreeSpaceInStack;
	// FreeSpaceInTotal = ((FreeStacksPerGroup[Group] == 0) && (FreeSpaceInTotal == 10)) ? 0 : FreeSpaceInTotal;
	// MaxItemsTotal == 0 means that there is now limit regarding total item count and all free space can be used
	FreeSpaceInTotal = (MaxItemsTotal == 0) ? FreeSpaceInTotal : FMath::Min(FreeSpaceInTotal, MaxItemsTotal);
	FreeSpaceInTotal = FMath::Max(0, FreeSpaceInTotal);

	// Add to Inventory
	const int32 OldCount = Entry->Count;
	const int32 NewCount = (PickupItemCount > FreeSpaceInTotal) ? OldCount + FreeSpaceInTotal : OldCount + PickupItemCount;
	Entry->Count = NewCount;

	// Update FreeStacksPerGroup
	UpdateFreeStacks(Group, MaxItemsPerStack, OldCount, NewCount);

	const int32 ItemsAdded = NewCount - OldCount;
	return ItemsAdded;
}

int32 UInventoryComponent::RemoveItems(EInventoryGroup Group, int32 MaxItemsPerStack, int32 Count, const int Index)
{
	const int32 ItemsRemoved = FMath::Min(Count, Entries[Index].Count);
	const int32 OldCount = Entries[Index].Count;
	Entries[Index].Count -= ItemsRemoved;

	UpdateFreeStacks(Group, MaxItemsPerStack, OldCount, Entries[Index].Count);
	
	if(Entries[Index].Count == 0)
	{
		Entries.RemoveAt(Index);
	}
	
	return ItemsRemoved;
}

FInventoryEntry* UInventoryComponent::GetOrCreateEntry(UInventoryItemDefinition* PickupItem, EInventoryGroup Group)
{
	FInventoryEntry* InventoryEntry = Entries.FindByPredicate(
	[PickupItem](const FInventoryEntry& Entry)
		{
			return Entry.ItemDef->IsA(PickupItem->GetClass());
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
	// FreeStacksPerGroup[Group] -= 1;
	return InventoryEntry;
}

bool UInventoryComponent::TryAttachItem(UInventoryItemDefinition* ItemDef)
{
	if(const auto* Attachable = ItemDef->FindFragmentByClass<UInventoryFragment_AttachableItem>())
	{
		AItem* Item = GetWorld()->SpawnActor<AItem>(Attachable->ItemClass, FTransform());
		GetEiraCharacterOwner()->AttachToSocket(Item, Attachable->SocketName);
		return true;
	}
	return false;
}

