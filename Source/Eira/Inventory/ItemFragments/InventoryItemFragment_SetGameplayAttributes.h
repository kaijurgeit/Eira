// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryItemFragment.h"
#include "UObject/Object.h"
#include "InventoryItemFragment_SetGameplayAttributes.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UInventoryItemFragment_SetGameplayAttributes : public UInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	/** Specify Attribute, Value is Item Count */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag Attribute;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, float> AttributeMap;
};
