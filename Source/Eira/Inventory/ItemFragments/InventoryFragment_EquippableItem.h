// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "UObject/Object.h"
#include "InventoryFragment_EquippableItem.generated.h"

class UEiraGameplayAbility;
/**
 * 
 */
UCLASS()
class EIRA_API UInventoryFragment_EquippableItem : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> Equipment;

	UPROPERTY(EditAnywhere, Category=Equipment)
	TArray<TObjectPtr<UEiraGameplayAbility>> AbilitiesToGrant;
	
	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;
	
	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};
