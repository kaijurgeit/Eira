// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "InventoryItemFragment.h"
#include "UObject/Object.h"
#include "UInventoryItemFragment_SetAttribute.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UUInventoryItemFragment_SetAttribute : public UInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayAttribute, int32> Attributes;
};
