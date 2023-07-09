// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "UObject/Object.h"
#include "UInventoryFragment_InventoryEntryLayout.generated.h"

UENUM(BlueprintType)
enum class EInventoryGroup
{
	None,
	Quests,
	Weapons,
	Props,
	Resources,
	Craft
};

/**
 * 
 */
UCLASS()
class EIRA_API UUInventoryFragment_InventoryEntryLayout : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EInventoryGroup Group = EInventoryGroup::None;
	
	UPROPERTY(EditAnywhere)
	int32 MaxItemsPerStack = 0;
	
	UPROPERTY(EditAnywhere)
	int32 MaxItemsTotal = 0;
};
