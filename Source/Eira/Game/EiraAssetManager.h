// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "EiraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class EIRA_API UEiraAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UEiraAssetManager();

	static UEiraAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
