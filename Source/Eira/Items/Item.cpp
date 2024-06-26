﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/MaterialBillboardComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemDefinition.h"

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

void AItem::Attach()
{	
	IconBillboardComponent->SetHiddenInGame(true);	
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void AItem::UnAttach()
{
	IconBillboardComponent->SetHiddenInGame(false);	
	StaticMeshComponent->SetCollisionProfileName(TEXT("Interact"));
}

TArray<FInventoryClassEntry>& AItem::GetPickupInventory()
{	
	return StaticInventory;
}

void AItem::AddToInventory(UInventoryComponent* InventoryComponent)
{
	if(!InventoryComponent) return;
	
	for (FInventoryClassEntry& ItemClassEntry : StaticInventory)
	{
		const int32 ItemsAdded = InventoryComponent->AddItemDefinition(ItemClassEntry.ItemDef, ItemClassEntry.Count);
		ItemClassEntry.Count -= ItemsAdded;
	}
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

	for (auto InventoryClassEntry : StaticInventory)
	{
		auto ItemDef = GetMutableDefault<UInventoryItemDefinition>(InventoryClassEntry.ItemDef);
		if(!ItemDef->ItemClass)
		{
			ItemDef->ItemClass = this->GetClass();			
		}
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}