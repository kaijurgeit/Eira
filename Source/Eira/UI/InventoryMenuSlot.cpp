// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenuSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryMenuSlot::UpdateCount(int ItemCount, int MaxItemCount)
{
	Count = ItemCount;
	if(UTextBlock* TextBlock = Cast<UTextBlock>(CountText))
	{
		FString String ;
		if(ItemCount == 0 || MaxItemCount == 1)
		{
			String = FString("");
		}
		else
		{
			String = FString::Printf(TEXT("%d/%d"), ItemCount, MaxItemCount);
		}
		const FText Text = FText::FromString(String);
		TextBlock->SetText(Text);
	}
}

void UInventoryMenuSlot::UpdateIcon(int ItemCount, UTexture2D* IconTexture)
{		
	FSlateBrush Brush;
	Brush.SetImageSize(FVector2d(100.f, 100.f));
	Icon->SetBrush(Brush);
	Icon->SetBrushFromTexture(IconTexture);				
	Icon->SetBrushTintColor(HighlightColor);	
	
}
