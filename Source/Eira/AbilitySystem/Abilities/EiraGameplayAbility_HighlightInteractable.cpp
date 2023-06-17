// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraGameplayAbility_HighlightInteractable.h"

#include "AbilitySystemComponent.h"
#include "Engine/Engine.h"

void UEiraGameplayAbility_HighlightInteractable::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo,
                                                          const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString(__FUNCTION__));
	}

	if(bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
