// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenuSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void URadialMenuSlot::UpdateCount(int Count, int Max)
{
	if(UTextBlock* TextBlock = Cast<UTextBlock>(CountText))
	{
		FString String;
		if(Count == 0 || Max < 2)
		{
			String = FString("XXX");
		}
		else
		{
			String = FString::Printf(TEXT("%d/%d"), Count, Max);						
		}
		const FText Text = FText::FromString(String);
		TextBlock->SetText(Text);
		TextBlock->SetColorAndOpacity(DefaultColor);
	}
	
}

void URadialMenuSlot::UpdateIcon(int Count, UTexture2D* IconTexture)
{
	// If no item in Slot remove Icon
	if(Count == 0)
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
