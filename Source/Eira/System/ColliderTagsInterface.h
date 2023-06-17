// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ColliderTagsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UColliderTagsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EIRA_API IColliderTagsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<UShapeComponent*> GetCollidersThatHaveTags(FGameplayTagContainer ColliderTags);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UShapeComponent* GetColliderThatHasTag(FGameplayTag ColliderTag);
};
