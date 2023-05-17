// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/Object.h"
#include "EiraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 *
 * 
 */
UCLASS()
class EIRA_API UEiraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
		
public:
	UEiraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Health */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UEiraAttributeSet, Health);
	
	UPROPERTY(EditDefaultsOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UEiraAttributeSet, MaxHealth);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	/** Stamina */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UEiraAttributeSet, Stamina);

	UPROPERTY(EditDefaultsOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UEiraAttributeSet, MaxStamina);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	
	/** AttackPower */
	UPROPERTY(EditDefaultsOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UEiraAttributeSet, AttackPower);

	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);
};
