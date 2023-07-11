﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "InventoryFragment_DropableItem.generated.h"

class AItem;
/**
 * 
 */
UCLASS()
class EIRA_API UInventoryFragment_DropableItem : public UInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Inventory)
	TSubclassOf<AItem> DropableItem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Inventory)
	FTransform SpawnTransform;
};
