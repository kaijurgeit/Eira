// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryItemDefinition.generated.h"

class AItem;
class UInventoryItemFragment;
/**
 * 
 */
UCLASS(Blueprintable)
class EIRA_API UInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Inventory, Instanced)
	TArray<TObjectPtr<UInventoryItemFragment>> Fragments = TArray<TObjectPtr<UInventoryItemFragment>>();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Inventory)
	TSubclassOf<AItem> ItemClass;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
};
