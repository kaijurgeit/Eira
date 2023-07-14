// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "UObject/Object.h"
#include "InventoryFragment_InventoryEntryLayout.generated.h"

UENUM(BlueprintType)
enum class EInventoryGroup
{
	None,
	Quests,
	Weapons,
	Props,
	Resources,
	Craft
};

/**
 * 
 */
UCLASS(BlueprintType)
class EIRA_API UInventoryFragment_InventoryEntryLayout : public UInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryGroup Group = EInventoryGroup::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxItemsPerStack = 1;

	// If set to 0 there is no limit
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxItemsTotal = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> IconTexture;

protected:
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	bool IsEditable() const;
};
