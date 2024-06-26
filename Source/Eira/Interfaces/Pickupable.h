﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailTreeNode.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "Pickupable.generated.h"

class UInventoryComponent;
class UInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FInventoryClassEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryItemDefinition> ItemDef = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;
};

USTRUCT(BlueprintType)
struct FInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInventoryItemDefinition> ItemDef = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;
};

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EIRA_API IPickupable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual TArray<FInventoryClassEntry>& GetPickupInventory() = 0;	
};

UCLASS()
class UPickupableStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	static TScriptInterface<IPickupable> GetFirstPickupableFromActor(AActor* Actor);

	//TODO: Do we really want a IPickupable or just an AItem? We have to cast it anyways to AItem, see GA_Pickup to destroy the empty Actor
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, meta = (WorldContext = "Ability"))
	static void AddPickupToInventory(UInventoryComponent* InventoryComponent, TScriptInterface<IPickupable> Pickup);
};
