// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemDefinition.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Pickupable.h"
#include "ItemFragments/InventoryFragment_InventoryEntryLayout.h"

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

	UPROPERTY(BlueprintAssignable)
	FUpdateInventory UpdateInventory;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	int32 AddItemDefinition(const FInventoryEntry& PickupEntry);
	
	UFUNCTION(BlueprintCallable)
	int32 RemoveItemDefinition(const FInventoryEntry& DropEntry);

	UFUNCTION(BlueprintCallable)
	void Select(const UInventoryItemDefinition* ItemDef);
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EInventoryGroup, int32> MaxStacksPerGroup = {{ EInventoryGroup::Resources, 3 }};

private:
	UPROPERTY()
	TArray<FInventoryEntry> Entries;	
	
	UPROPERTY()
	TMap<EInventoryGroup, int32> FreeStacksPerGroup;

	UPROPERTY()
	TObjectPtr<AEiraCharacter> EiraCharacterOwner;
	
	int32 AddItems(FInventoryEntry* Entry, EInventoryGroup Group, int32 MaxItemsPerStack, int32 MaxItemsTotal, int32 PickupItemCount);
	FInventoryEntry* GetOrCreateEntry(UInventoryItemDefinition* PickupItem, EInventoryGroup Group);	
	AEiraCharacter* GetEiraCharacterOwner();
};
