// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

class UGameplayTagsManager;

/**
 * EiraGameplayTags
 *
 *	Singleton containing native gameplay tags.
 */
struct FEiraGameplayTags
{
public:
	
	static const FEiraGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	// Input Tags
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Fire;

protected:

	//Registers all of the tags with the GameplayTags Manager
	void AddAllTags(UGameplayTagsManager& Manager);

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:
	
	static FEiraGameplayTags GameplayTags;	
};
