// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/EiraGameplayAbility.h"
#include "UObject/Object.h"
#include "UEiraGameplayAbility_InteractableSource.generated.h"

class IInteractableSource;

/**
 * 
 */
UCLASS()
class EIRA_API UUEiraGameplayAbility_InteractableSource : public UEiraGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	TScriptInterface<IInteractableSource> InteractableSource;
	
protected:
	virtual void SetInteractableSource(const FGameplayAbilityActorInfo* ActorInfo);
};
