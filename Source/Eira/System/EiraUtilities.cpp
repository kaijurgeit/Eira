// Fill out your copyright notice in the Description page of Project Settings.


#include "EiraUtilities.h"

void UEiraUtilities::OrderByDistance(AActor* OriginActor, TArray<AActor*> InArray, TArray<AActor*>& OutArray)
{
	InArray.Sort([OriginActor](const AActor& A, const AActor& B)
	{
		const float DistanceA = A.GetDistanceTo(OriginActor);
		const float DistanceB = B.GetDistanceTo(OriginActor);
		return DistanceA < DistanceB; 
	});
	OutArray = InArray;
}
