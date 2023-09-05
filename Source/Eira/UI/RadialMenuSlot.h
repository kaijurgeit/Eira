// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemFragments/InventoryItemFragment_RadialMenu.h"
#include "RadialMenuSlot.generated.h"

class UTextLayoutWidget;
class UImage;
/**
 * 
 */
UCLASS()
class EIRA_API URadialMenuSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateCount(int ItemCount, int MaxItemCount);

	UFUNCTION(BlueprintCallable)
	void UpdateIcon(int ItemCount, UTexture2D* IconTexture);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ERadialMenuGroup Group;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UInventoryItemDefinition> ItemDef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextLayoutWidget> CountText;

private:
	FLinearColor DefaultColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	FLinearColor NoColor = FLinearColor(1.f, 1.f, 1.f, 0.f);
	FLinearColor HighlightColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
};
