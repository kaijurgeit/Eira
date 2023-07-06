// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Pickupable.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "FullMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UFullMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UFUNCTION()
	void UpdateInventory(FInventoryEntry InventoryEntry);
};
