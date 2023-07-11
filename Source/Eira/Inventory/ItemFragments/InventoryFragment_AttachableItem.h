// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "UObject/Object.h"
#include "InventoryFragment_AttachableItem.generated.h"

class AItem;

/**
 * 
 */
UCLASS()
class EIRA_API UInventoryFragment_AttachableItem : public UInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Inventory)
	TSubclassOf<AItem> ItemClass;
	
	UPROPERTY(EditAnywhere, Category=Inventory)
	FName AttachSocket;
	
	UPROPERTY(EditAnywhere, Category=Inventory)
	FTransform AttachTransform;
};
