// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UEiraGameplayAbility_InteractableSource.h"
#include "AbilitySystem/EiraGameplayAbility.h"
#include "EiraGameplayAbility_Interactable_Update.generated.h"

class IInteractableSource;
class IInteractableTarget;
/**
 * 
 */
UCLASS()
class EIRA_API UEiraGameplayAbility_Interactable_Update : public UUEiraGameplayAbility_InteractableSource
{
	GENERATED_BODY()

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;	

	UFUNCTION(BlueprintCallable)
	void UpdateInteractableTarget(const FGameplayAbilityActorInfo ActorInfo);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	bool bActivateAbilityOnGranted = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag InteractColliderTag;

private:
	TArray<TObjectPtr<AActor>> InteractableTargets;	
	TObjectPtr<UShapeComponent> InteractCollider;
	
	bool BindToAddRemoveOverlappingInteractActors(const FGameplayAbilityActorInfo* ActorInfo);
	AActor* GetNearestInteractActor(const FGameplayAbilityActorInfo* ActorInfo);
	void HighlightInteractableTarget(bool IsHighlighted);

	UFUNCTION()
	void AddInteractableTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void RemoveInteractableTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
