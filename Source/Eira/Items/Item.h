// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableTarget.h"
#include "Interfaces/Pickupable.h"
#include "Item.generated.h"

class UMaterialBillboardComponent;

UCLASS()
class EIRA_API AItem : public AActor, public IInteractableTarget, public IPickupable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TheGame", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TheGame", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialBillboardComponent> IconBillboardComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	// Sets default values for this actor's properties
	AItem();
	virtual void Highlight(bool IsHightlighted) override;
	virtual void Interact() override;
	void Attach();
	void UnAttach();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = Inventory)
	TObjectPtr<UTexture2D> Icon;

	virtual TArray<FInventoryClassEntry>& GetPickupInventory() override;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Inventory)
	TArray<FInventoryClassEntry> StaticInventory;
};
