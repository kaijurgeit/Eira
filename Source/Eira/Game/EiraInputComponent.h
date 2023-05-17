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

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UEiraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
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

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UEiraInputComponent::BindAbilityActions(const UEiraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for(FEiraInputAction Action : InputConfig->AbilityInputActions)
	{
		if(PressedFunc)
		{
			BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
		}
		
		if(ReleasedFunc)
		{
			BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
		}
	}
}
