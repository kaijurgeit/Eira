// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraAttributeSet.h"
#include "Net/UnrealNetwork.h"

UEiraAttributeSet::UEiraAttributeSet()
{
}

void UEiraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UEiraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEiraAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEiraAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
}

void UEiraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEiraAttributeSet, Health, OldHealth);
}

void UEiraAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEiraAttributeSet, Stamina, OldStamina);
}

void UEiraAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEiraAttributeSet, AttackPower, OldAttackPower);
}
