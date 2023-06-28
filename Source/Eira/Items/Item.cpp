// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/MaterialBillboardComponent.h"


// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionProfileName(TEXT("Interact"));

	IconBillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>("Icon Billboard");
	IconBillboardComponent->SetupAttachment(GetRootComponent());

	FMaterialSpriteElement Element;
	Element.BaseSizeX = 5.f;
	Element.BaseSizeY = 5.f;
	IconBillboardComponent->SetElements(TArray({Element}));
	
	IconBillboardComponent->SetHiddenInGame(true);
}

void AItem::Highlight(bool IsHightlighted)
{	
	IconBillboardComponent->SetHiddenInGame(!IsHightlighted);
}

void AItem::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}


// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	// Set IconImage in Billboard MaterialInstance
	UMaterialInterface* Material = IconBillboardComponent->GetMaterial(0);
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	DynamicMaterial->SetTextureParameterValue("Icon", Icon);
	IconBillboardComponent->SetMaterial(0, DynamicMaterial);
	const FVector IconLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
	IconBillboardComponent->SetWorldLocation(IconLocation);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}