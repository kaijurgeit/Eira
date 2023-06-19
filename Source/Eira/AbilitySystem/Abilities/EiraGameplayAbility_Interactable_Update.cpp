// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraGameplayAbility_Interactable_Update.h"

#include "AbilitySystemComponent.h"
#include "Components/ShapeComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/ColliderTagsInterface.h"
#include "Interfaces/InteractableSource.h"
#include "Interfaces/InteractableTarget.h"
#include "System/EiraUtilities.h"

void UEiraGameplayAbility_Interactable_Update::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo,
															 const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UEiraGameplayAbility_Interactable_Update::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	BindToAddRemoveOverlappingInteractActors(ActorInfo);
}

void UEiraGameplayAbility_Interactable_Update::UpdateInteractableTarget(const FGameplayAbilityActorInfo ActorInfo)
{
	AActor* NewInteractableTarget = GetNearestInteractActor(&ActorInfo);
	if(NewInteractableTarget == InteractableSource->GetInteractableTargetActor())
	{
		return;
	}

	HighlightInteractableTarget(false);
	InteractableSource->SetInteractableTargetActor(NewInteractableTarget);
	HighlightInteractableTarget(true);	
}

bool UEiraGameplayAbility_Interactable_Update::BindToAddRemoveOverlappingInteractActors(const FGameplayAbilityActorInfo* ActorInfo)
{
	if(const IColliderTagsInterface* ColliderTagsInterface = Cast<IColliderTagsInterface>(ActorInfo->OwnerActor))
	{
		InteractCollider = ColliderTagsInterface->Execute_GetColliderThatHasTag(ActorInfo->OwnerActor.Get(), InteractColliderTag);
		ensureAlwaysMsgf(IsValid(InteractCollider), TEXT("No IsValid(InteractCollider) set in : %s"), *GetName());
		if(!IsValid(InteractCollider))
		{
			return false;
		}
		InteractCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AddInteractableTarget);
		InteractCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::RemoveInteractableTarget);
	}
	return true;
}

void UEiraGameplayAbility_Interactable_Update::HighlightInteractableTarget(bool IsHighlighted)
{
	if(IInteractableTarget* InteractableTarget = Cast<IInteractableTarget>(InteractableSource->GetInteractableTargetActor()))
	{
		InteractableTarget->Highlight(IsHighlighted);		
	}
}


AActor* UEiraGameplayAbility_Interactable_Update::GetNearestInteractActor(const FGameplayAbilityActorInfo* ActorInfo)
{
	UEiraUtilities::OrderByDistance(ActorInfo->OwnerActor.Get(), InteractableTargets, InteractableTargets);

	if(InteractableTargets.IsEmpty())
	{
		return nullptr;
	}
	return InteractableTargets[0];
}

void UEiraGameplayAbility_Interactable_Update::AddInteractableTarget(UPrimitiveComponent* OverlappedComponent,
                                                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                 const FHitResult& SweepResult)
{
	InteractableTargets.Add(OtherActor);
}

void UEiraGameplayAbility_Interactable_Update::RemoveInteractableTarget(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	InteractableTargets.Remove(OtherActor);
}
