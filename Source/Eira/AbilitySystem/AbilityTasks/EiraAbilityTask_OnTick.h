// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "EiraAbilityTask_OnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

/**
 * Task for abilities that supply tick and its' delta time.
 */
UCLASS()
class EIRA_API UEiraAbilityTask_OnTick : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;

public:
	UEiraAbilityTask_OnTick(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UEiraAbilityTask_OnTick* AbilityTaskOnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

private:
	float TaskDeltaTime;
};
