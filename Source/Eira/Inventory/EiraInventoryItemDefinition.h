// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EiraInventoryItemDefinition.generated.h"

class UEiraInventoryItemInstance;

// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class EIRA_API UEiraInventoryItemFragment : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnInstanceCreated(UEiraInventoryItemInstance* Instance) const {}
};


/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class EIRA_API UEiraInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	TArray<TObjectPtr<UEiraInventoryItemFragment>> Fragments;

	const UEiraInventoryItemFragment* FindFragmentByClass(TSubclassOf<UEiraInventoryItemFragment> FragmentClass) const;
};

UCLASS()
class UEiraInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UEiraInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UEiraInventoryItemDefinition> ItemDef, TSubclassOf<UEiraInventoryItemFragment> FragmentClass);
};
