// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraAssetManager.h"

#include "EiraGameplayTags.h"

UEiraAssetManager::UEiraAssetManager()
{
}

UEiraAssetManager& UEiraAssetManager::Get()
{
	check(GEngine);

	UEiraAssetManager* EiraAssetManager = Cast<UEiraAssetManager>(GEngine->AssetManager);
	
	return *EiraAssetManager;
}

void UEiraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Load Native Tags
	FEiraGameplayTags::InitializeNativeTags();
}


