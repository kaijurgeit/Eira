// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraGameplayAbility_HighlightInteractable.h"

#include "AbilitySystemComponent.h"
#include "Components/ShapeComponent.h"
#include "Engine/Engine.h"
#include "Interfaces/ColliderTagsInterface.h"
#include "Interfaces/InteractableSource.h"
#include "System/EiraUtilities.h"

UEiraGameplayAbility_HighlightInteractable::UEiraGameplayAbility_HighlightInteractable(
	const FObjectInitializer& ObjectInitializer)
		: NearbyInteractActors(TArray<TObjectPtr<AActor>>())
{
}

bool UEiraGameplayAbility_HighlightInteractable::BindToAddRemoveOverlappingInteractActors(const FGameplayAbilityActorInfo& ActorInfo, FGameplayTag ColliderTag)
{
	if(IColliderTagsInterface* ColliderTagsInterface = Cast<IColliderTagsInterface>(ActorInfo.OwnerActor))
	{
		InteractCollider = ColliderTagsInterface->Execute_GetColliderThatHasTag(ActorInfo.OwnerActor.Get(), ColliderTag);
		ensureAlwaysMsgf(IsValid(InteractCollider), TEXT("No IsValid(InteractCollider) set in : %s"), *GetName());
		if(!IsValid(InteractCollider))
		{
			return false;
		}
		InteractCollider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AddInteractable);
		InteractCollider->OnComponentEndOverlap.AddDynamic(this, &ThisClass::RemoveInteractable);
	}
	return true;
}

void UEiraGameplayAbility_HighlightInteractable::SetInteractableSource(const FGameplayAbilityActorInfo ActorInfo)
{
	if(IInteractableSource* PureInteractableSource = Cast<IInteractableSource>(ActorInfo.OwnerActor))
	{
		InteractableSource.SetInterface(PureInteractableSource);
		InteractableSource.SetObject(Cast<UObject>(PureInteractableSource));
	}
}

void UEiraGameplayAbility_HighlightInteractable::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo,
                                                             const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if(bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UEiraGameplayAbility_HighlightInteractable::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString(__FUNCTION__));
	}
}

TArray<AActor*> UEiraGameplayAbility_HighlightInteractable::GetNearbyInteractActors()
{
	return NearbyInteractActors;
}

AActor* UEiraGameplayAbility_HighlightInteractable::GetNearestInteractActor(const FGameplayAbilityActorInfo ActorInfo)
{
	UEiraUtilities::OrderByDistance(ActorInfo.OwnerActor.Get(), NearbyInteractActors, NearbyInteractActors);

	if(NearbyInteractActors.IsEmpty())
	{
		return nullptr;
	}

	return NearbyInteractActors[0];
}

void UEiraGameplayAbility_HighlightInteractable::AddInteractable(UPrimitiveComponent* OverlappedComponent,
                                                                 AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                                 const FHitResult& SweepResult)
{
	NearbyInteractActors.Add(OtherActor);
}

void UEiraGameplayAbility_HighlightInteractable::RemoveInteractable(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	NearbyInteractActors.Remove(OtherActor);
}
