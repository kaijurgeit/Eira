// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemFragment_SetStats.h"


int32 UInventoryItemFragment_SetStats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
