﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UEiraGameplayAbility_InteractableSource.h"
#include "AbilitySystem/EiraGameplayAbility.h"
#include "EiraGameplayAbility_HighlightInteractable.generated.h"

class IInteractableSource;
class IInteractableTarget;
/**
 * 
 */
UCLASS()
class EIRA_API UEiraGameplayAbility_HighlightInteractable : public UUEiraGameplayAbility_InteractableSource
{
	GENERATED_BODY()

public:
	UEiraGameplayAbility_HighlightInteractable(const FObjectInitializer& ObjectInitializer);
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	
	UFUNCTION(BlueprintCallable)
	bool BindToAddRemoveOverlappingInteractActors(const FGameplayAbilityActorInfo& ActorInfo, FGameplayTag ColliderTag);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ability")
	bool bActivateAbilityOnGranted = true;	

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetNearbyInteractActors();

	UFUNCTION(BlueprintCallable)
	AActor* GetNearestInteractActor(const FGameplayAbilityActorInfo ActorInfo);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> NearbyInteractActors;
	
	UPROPERTY()
	TObjectPtr<UShapeComponent> InteractCollider;

	UFUNCTION()
	void AddInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void RemoveInteractable(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
