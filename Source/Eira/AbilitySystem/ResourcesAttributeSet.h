// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/Object.h"
#include "ResourcesAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class EIRA_API UResourcesAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Gold */
	UPROPERTY(EditDefaultsOnly, Category = "ResourceAttribute")
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UResourcesAttributeSet, Gold);
	
	/** Gold */
	UPROPERTY(EditDefaultsOnly, Category = "ResourceAttribute")
	FGameplayAttributeData Silver;
	ATTRIBUTE_ACCESSORS(UResourcesAttributeSet, Silver);
};
