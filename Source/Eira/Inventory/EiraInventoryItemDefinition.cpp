// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraInventoryItemDefinition.h"

const UEiraInventoryItemFragment* UEiraInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UEiraInventoryItemFragment> FragmentClass) const
{
	if(FragmentClass != nullptr)
	{
		for(UEiraInventoryItemFragment* Fragment : Fragments)
		{
			if(Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

const UEiraInventoryItemFragment* UEiraInventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<UEiraInventoryItemDefinition> ItemDef, TSubclassOf<UEiraInventoryItemFragment> FragmentClass)
{
	if((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UEiraInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
