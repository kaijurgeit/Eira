// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "InventorySlot.generated.h"

class UTextLayoutWidget;

/**
 * 
 */
UCLASS(Blueprintable)
class EIRA_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextLayoutWidget> Count;
};
