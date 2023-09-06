// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "InventoryFragment_GrantGameplayAbility.generated.h"

class UGameplayAbility;

class UEiraGameplayAbility;
/**
 * 
 */
UCLASS()
class EIRA_API UInventoryFragment_GrantGameplayAbility : public UInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;
};
