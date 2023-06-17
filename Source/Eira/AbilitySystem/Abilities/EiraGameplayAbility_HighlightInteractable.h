// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EiraGameplayAbility.h"
#include "EiraGameplayAbility_HighlightInteractable.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UEiraGameplayAbility_HighlightInteractable : public UEiraGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bActivateAbilityOnGranted = true;
};
