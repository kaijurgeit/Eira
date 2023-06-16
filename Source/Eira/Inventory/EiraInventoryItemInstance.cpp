// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraInventoryItemInstance.h"

#include "Net/UnrealNetwork.h"

void UEiraInventoryItemInstance:: SetItemDef(TSubclassOf<UEiraInventoryItemDefinition> InDef)
{	
	ItemDef = InDef;
}

void UEiraInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
}