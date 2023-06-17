// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/InteractTarget.h"
#include "Item.generated.h"

UCLASS()
class EIRA_API AItem : public AActor, public IInteractTarget
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	virtual void SetHighlight(bool IsHightlighted) override;
	virtual void Interact() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
