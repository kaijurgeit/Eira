// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "GameplayTagContainer.h"
#include "InventoryItemFragment_SetStats.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UInventoryItemFragment_SetStats : public UInventoryItemFragment
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, int32> InitialItemStats;

public:
	int32 GetItemStatByTag(FGameplayTag Tag) const;
};
