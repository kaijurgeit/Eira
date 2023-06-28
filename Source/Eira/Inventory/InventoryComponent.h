// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FInventoryEntry
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<UInventoryItemDefinition> ItemDef = nullptr;
	
	UPROPERTY()
	int32 StackCount = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class EIRA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<FInventoryEntry> Entries;
};
