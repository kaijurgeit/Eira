// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraInputConfig.h"

const UInputAction* UEiraInputConfig::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FEiraInputAction& NativeInputAction : NativeInputActions)
	{
		if (NativeInputAction.InputAction && NativeInputAction.InputTag == InputTag)
		{
			return  NativeInputAction.InputAction;
		}
	}

	return nullptr;
}
