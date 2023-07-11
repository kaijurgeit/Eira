// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "InventoryItemFragment_QuickAccess.generated.h"

class UInventoryItemDefinition;
class USizeBox;
class UQuickAccessSlot;


UENUM(BlueprintType)
enum class EQuickAccessGroup :uint8
{
	None,
	Melee1,
	Melee2,
	Ranged1,
	Ranged2,
	ShotgunShells,
	Flares,
	Back,
	Backpack
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSectorInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuickAccessMenu)
	USizeBox* SizeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuickAccessMenu)
	EQuickAccessGroup StorageName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuickAccessMenu)
	int Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = QuickAccessMenu)
	UQuickAccessSlot* Sector;	
	// UPROPERTY(BlueprintReadWrite, Category = QuickAccessMenu)
	// UInventoryItemDefinition ItemDefinition;	
};

/**
 * 
 */
UCLASS()
class EIRA_API UInventoryItemFragment_QuickAccess : public UInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuickAccessGroup Group;
};

