// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraPlayerController.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/EiraAbilitySystemComponent.h"


// Sets default values
AEiraPlayerController::AEiraPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UEiraAbilitySystemComponent* AEiraPlayerController::GetEiraAbilitySystemComponent() const
{	
	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetPawn());
	return (ASI ? Cast<UEiraAbilitySystemComponent>(ASI->GetAbilitySystemComponent()) : nullptr);
}

// Called when the game starts or when spawned
void AEiraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEiraPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if(UEiraAbilitySystemComponent* EiraASC = GetEiraAbilitySystemComponent())
	{
		EiraASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

// Called every frame
void AEiraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

