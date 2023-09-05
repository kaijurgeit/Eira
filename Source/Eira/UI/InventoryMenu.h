// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Pickupable.h"
#include "Inventory/ItemFragments/InventoryFragment_InventoryMenu.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UInventorySlot;
class UGridPanel;
/**
 * 
 */
UCLASS(Blueprintable)
class EIRA_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SlotClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UGridPanel> GridWeapons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UGridPanel> GridResources;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UGridPanel> GridProps;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UGridPanel> GridCraft;
	
	UFUNCTION(BlueprintNativeEvent)
	void UpdateInventory(const TArray<FInventoryEntry>& Inventory);

	UGridPanel* SelectGridPanel(const UInventoryFragment_InventoryMenu* Layout);
	UFUNCTION(BlueprintCallable)
	int CreateInventorySlots(const FInventoryEntry& Entry, int32 ColCount, int32 StartIndex = 0);


private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	FLinearColor HighlightColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
};
