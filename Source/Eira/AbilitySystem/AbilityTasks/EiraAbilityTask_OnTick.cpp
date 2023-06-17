// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraAbilityTask_OnTick.h"

UEiraAbilityTask_OnTick::UEiraAbilityTask_OnTick(const FObjectInitializer& ObjectInitializer)
{
	bTickingTask = true;
}

UEiraAbilityTask_OnTick* UEiraAbilityTask_OnTick::AbilityTaskOnTick(UGameplayAbility* OwningAbility,
	FName TaskInstanceName)
{
	UEiraAbilityTask_OnTick* MyObj = NewAbilityTask<UEiraAbilityTask_OnTick>(OwningAbility);
	return MyObj;
}

void UEiraAbilityTask_OnTick::Activate()
{
	Super::Activate();
}

void UEiraAbilityTask_OnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	OnTick.Broadcast(DeltaTime);
}
