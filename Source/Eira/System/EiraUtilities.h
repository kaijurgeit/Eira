// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "EiraUtilities.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UEiraUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Helpers)
	static void OrderByDistance(AActor* OriginActor, TArray<AActor*>& OutArray);
};
