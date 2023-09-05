// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFragment_InventoryMenu.h"

bool UInventoryFragment_InventoryMenu::CanEditChange(const FProperty* InProperty) const
{
	const bool ParentValue = Super::CanEditChange(InProperty);

	if(InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UInventoryFragment_InventoryMenu, MaxItemsPerStack))
	{
		return ParentValue && IsEditable(); 
	}
	
	if(InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UInventoryFragment_InventoryMenu, MaxItemsTotal))
	{
		return ParentValue && IsEditable(); 
	}
	
	return Super::CanEditChange(InProperty);
}

bool UInventoryFragment_InventoryMenu::IsEditable() const
{
	return Group == EInventoryGroup::Resources || Group == EInventoryGroup::Props || Group == EInventoryGroup::Craft;
}
