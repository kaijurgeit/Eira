// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EiraPlayerController.generated.h"

class UEiraAbilitySystemComponent;

UCLASS()
class EIRA_API AEiraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEiraPlayerController();
	UEiraAbilitySystemComponent* GetEiraAbilitySystemComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
