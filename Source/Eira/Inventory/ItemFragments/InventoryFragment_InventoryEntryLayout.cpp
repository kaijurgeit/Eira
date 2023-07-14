// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFragment_InventoryEntryLayout.h"

bool UInventoryFragment_InventoryEntryLayout::CanEditChange(const FProperty* InProperty) const
{
	const bool ParentValue = Super::CanEditChange(InProperty);

	if(InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UInventoryFragment_InventoryEntryLayout, MaxItemsPerStack))
	{
		return ParentValue && IsEditable(); 
	}
	
	if(InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UInventoryFragment_InventoryEntryLayout, MaxItemsTotal))
	{
		return ParentValue && IsEditable(); 
	}
	
	return Super::CanEditChange(InProperty);
}

bool UInventoryFragment_InventoryEntryLayout::IsEditable() const
{
	return Group == EInventoryGroup::Resources || Group == EInventoryGroup::Props || Group == EInventoryGroup::Craft;
}
