// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EiraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "UObject/Object.h"
#include "EiraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UEiraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UEiraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);	
};

template <class UserClass, typename FuncType>
void UEiraInputComponent::BindNativeAction(const UEiraInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if(const UInputAction* IA = InputConfig->FindInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}
