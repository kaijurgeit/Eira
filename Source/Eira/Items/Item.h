// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UMaterialBillboardComponent;
class APickup;
class UTexture;
class UCapsuleComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EStorageName :uint8
{
	None,
	Melee1,
	Melee2,
	Ranged1,
	Ranged2,
	ShotgunShells,
	Flares,
	Back,
	Backpack
};

UCLASS()
class EIRA_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();
	virtual void Tick(float DeltaTime) override;
	// virtual  EInteractType Interact() override;
	void Attach();
	void UnAttach();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TheGame")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TheGame")
	EStorageName StorageName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "TheGame")
	int Count = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TheGame")
	bool Consumable = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TheGame")
	EStorageName AssociatedStorageName = EStorageName::None;

	UMeshComponent* GetMesh() { return StaticMeshComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TheGame", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "TheGame", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialBillboardComponent> IconBillboardComponent;

	UFUNCTION()
	void ShowIcon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()					
	void HideIcon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
