// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EiraInventoryManagerComponent.generated.h"


USTRUCT(BlueprintType)
struct FEiraInventoryEntry
{
	GENERATED_BODY()

	FEiraInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FEiraInventoryList;
	friend UEiraInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UEiraInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FEiraInventoryList
{
	GENERATED_BODY()

	FEiraInventoryList()
		: OwnerComponent(nullptr)
	{		
	}
	
	FEiraInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{		
	}

	UEiraInventoryItemInstance* AddEntry(TSubclassOf<UEiraInventoryItemDefinition> ItemDef, int32 StackCount);

	void RemoveEntry(UEiraInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FEiraInventoryEntry& Entry, int32 OldCount, int32 NewCount);

	friend UEiraInventoryManagerComponent;
	
	// Replicated list of items
	UPROPERTY()
	TArray<FEiraInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};


/**
 * Manages an inventory
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIRA_API UEiraInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEiraInventoryManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UEiraInventoryItemInstance* AddItemDefinition(TSubclassOf<UEiraInventoryItemDefinition> ItemDef, int32 StackCount = 1);

private:
	UPROPERTY(Replicated)
	FEiraInventoryList InventoryList;
};