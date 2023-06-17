// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UObject/Object.h"
#include "EiraGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EEiraAbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	// OnSpawn
};

/**
 * 
 */
UCLASS()
class EIRA_API UEiraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	EEiraAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }	
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;


protected:	
	// Defines how this ability is meant to activate.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lyra|Ability Activation")
	EEiraAbilityActivationPolicy ActivationPolicy;
};
