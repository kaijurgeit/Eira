// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "InventoryMenuSlot.generated.h"

class UButton;
class UInventoryItemDefinition;
class UTextLayoutWidget;
class UImage;

/**
 * 
 */
UCLASS(Blueprintable)
class EIRA_API UInventoryMenuSlot : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly)
	int Count = 1;
	
	UFUNCTION(BlueprintCallable)
	void UpdateCount(int ItemCount, int MaxItemCount);

	UFUNCTION(BlueprintCallable)
	void UpdateIcon(int ItemCount, UTexture2D* IconTexture);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextLayoutWidget> CountText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventoryItemDefinition> ItemDef;

	UPROPERTY(EditDefaultsOnly)
	int Foo;
	
private:	
	FLinearColor HighlightColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
};
