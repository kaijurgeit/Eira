﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableSource.generated.h"

class IInteractableTarget;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UInteractableSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EIRA_API IInteractableSource
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetInteractableTargetActor(AActor* Value) {};
	
	UFUNCTION(BlueprintCallable)
	virtual AActor* GetInteractableTargetActor() { return nullptr; };
};
