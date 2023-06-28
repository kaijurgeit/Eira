// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryItemDefinition.generated.h"

class UInventoryItemFragment;
/**
 * 
 */
UCLASS()
class EIRA_API UInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Inventory)
	TArray<TObjectPtr<UInventoryItemFragment>> Fragments;
};
