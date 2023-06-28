// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraGameplayAbility_Interactable.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Interfaces/InteractableSource.h"

void UEiraGameplayAbility_Interactable::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	SetInteractableSource(ActorInfo);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UEiraGameplayAbility_Interactable::SetInteractableSource(const FGameplayAbilityActorInfo* ActorInfo)
{
	if(IInteractableSource* PureInteractableSource = Cast<IInteractableSource>(ActorInfo->OwnerActor))
	{
		InteractableSource.SetInterface(PureInteractableSource);
		InteractableSource.SetObject(Cast<UObject>(PureInteractableSource));
	}
}
