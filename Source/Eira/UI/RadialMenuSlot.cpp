// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenuSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void URadialMenuSlot::UpdateCount(int ItemCount, int MaxItemCount)
{
	if(UTextBlock* TextBlock = Cast<UTextBlock>(CountText))
	{
		FString String;
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
		TextBlock->SetColorAndOpacity(DefaultColor);
	}
	
}

void URadialMenuSlot::UpdateIcon(int ItemCount, UTexture2D* IconTexture)
{
	// If no item in Slot remove Icon
	if(ItemCount == 0)
	{
		Icon->SetBrushTintColor(NoColor);
		return;
	}
	if(Icon)
	{
		Icon->SetBrushFromTexture(IconTexture);				
		Icon->SetBrushTintColor(HighlightColor);		
	}
}
