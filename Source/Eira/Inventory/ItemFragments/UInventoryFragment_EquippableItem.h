// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "UObject/Object.h"
#include "UInventoryFragment_EquippableItem.generated.h"

class UEiraGameplayAbility;
/**
 * 
 */
UCLASS()
class EIRA_API UUInventoryFragment_EquippableItem : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TSubclassOf<AActor> Equipment;

	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TArray<TObjectPtr<UEiraGameplayAbility>> AbilitiesToGrant;
	
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	FName AttachSocket;
	
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	FName AttachTransform;
};
