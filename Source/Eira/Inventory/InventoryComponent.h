// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "InventoryItemDefinition.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Pickupable.h"
#include "ItemFragments/InventoryFragment_InventoryMenu.h"

#include "InventoryComponent.generated.h"

class AEiraCharacter;
class UInventoryItemDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventory, const TArray<FInventoryEntry>&, InventoryEntry);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIRA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable)
	FUpdateInventory UpdateInventory;
	
	UFUNCTION(BlueprintCallable)
	int32 AddItemDefinition(TSubclassOf<UInventoryItemDefinition> ItemDefClass, int32 Count);

	UFUNCTION(BlueprintCallable)
	int32 RemoveItemDefinition(UInventoryItemDefinition* ItemDef, int32 Count);

	UFUNCTION(BlueprintCallable)
	void Select(const UInventoryItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable)
	void Deselect(TSubclassOf<UInventoryItemDefinition> ItemDefClass);

protected:	
	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TSubclassOf<UGameplayEffect> GESetAnyAttributeByCaller;
	
	// If 0, there are infinite Stacks e.g. Quests
	UPROPERTY(EditDefaultsOnly)
	TMap<EInventoryGroup, int32> MaxStacksPerGroup = {
		{ EInventoryGroup::Quests, 0 },
		{ EInventoryGroup::Weapons, 4 },
		{ EInventoryGroup::Props, 5 },
		{ EInventoryGroup::Resources, 3 },
		{ EInventoryGroup::Craft, 6 }
	};

private:
	UPROPERTY() TArray<FInventoryEntry> Entries;	
	UPROPERTY() TMap<EInventoryGroup, int32> FreeStacksPerGroup; 
	UPROPERTY() TObjectPtr<AEiraCharacter> EiraCharacterOwner;
	
	AEiraCharacter* GetEiraCharacterOwner();
	
	bool TryAttachItem(UInventoryItemDefinition* ItemDef);
	bool TryEquip(const UInventoryItemDefinition* ItemDef);
	bool TryGrantAbilites(const UInventoryItemDefinition* ItemDef);
	bool TrySetGameplayAttributes(UInventoryItemDefinition* ItemDef, int32 ItemCount);
	
	void UpdateFreeStacks(EInventoryGroup Group, int32 MaxItemsPerStack, int32 OldCount, int32 NewCount);
	int32 AddItems(FInventoryEntry* Entry, EInventoryGroup Group, int32 MaxItemsPerStack, int32 MaxItemsTotal, int32 PickupItemCount);
	int32 RemoveItems(EInventoryGroup Group, int32 MaxItemsPerStack, int32 Count, const int Index);
	FInventoryEntry* GetOrCreateEntry(UInventoryItemDefinition* PickupItem, EInventoryGroup Group);
};
