// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EiraInventoryItemInstance.generated.h"

class UEiraInventoryItemDefinition;

/**
 * 
 */
UCLASS()
class EIRA_API UEiraInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:	
	void SetItemDef(TSubclassOf<UEiraInventoryItemDefinition> InDef);

private:	
	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UEiraInventoryItemDefinition> ItemDef;
};
