// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/EiraInventoryItemDefinition.h"
#include "InventoryItemFragment_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UInventoryItemFragment_Equipment : public UEiraInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEquipment>
};
