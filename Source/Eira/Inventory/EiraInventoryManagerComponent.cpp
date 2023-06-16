// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraInventoryManagerComponent.h"

#include "EiraInventoryItemDefinition.h"
#include "EiraInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"


UEiraInventoryItemInstance* FEiraInventoryList::AddEntry(TSubclassOf<UEiraInventoryItemDefinition> ItemDef,
                                                         int32 StackCount)
{
	UEiraInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FEiraInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UEiraInventoryItemInstance>(OwnerComponent->GetOwner());
	NewEntry.Instance->SetItemDef(ItemDef);
	for(UEiraInventoryItemFragment* Fragment : GetDefault<UEiraInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if(Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;
	
	// MarkItemDirty(NewEntry);

	return Result;
}

void FEiraInventoryList::RemoveEntry(UEiraInventoryItemInstance* Instance)
{
}

void FEiraInventoryList::BroadcastChangeMessage(FEiraInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
}

// Sets default values for this component's properties
UEiraInventoryManagerComponent::UEiraInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UEiraInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}


// Called when the game starts
void UEiraInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEiraInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UEiraInventoryItemInstance* UEiraInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<UEiraInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UEiraInventoryItemInstance* Result = nullptr;
	if(ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
	}
	return Result;
}

