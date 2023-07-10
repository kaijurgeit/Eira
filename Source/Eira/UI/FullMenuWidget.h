// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Pickupable.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "FullMenuWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class EIRA_API UFullMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void UpdateInventory(const TArray<FInventoryEntry>& Inventory);

private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
