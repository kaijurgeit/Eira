// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/CapsuleComponent.h"
#include "Components/MaterialBillboardComponent.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionProfileName(TEXT("Interact"));
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::ShowIcon);
	StaticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::HideIcon);

	IconBillboardComponent = CreateDefaultSubobject<UMaterialBillboardComponent>("Icon Billboard");
	IconBillboardComponent->SetupAttachment(GetRootComponent());

	FMaterialSpriteElement Element;
	Element.BaseSizeX = 5.f;
	Element.BaseSizeY = 5.f;
	IconBillboardComponent->SetElements(TArray<FMaterialSpriteElement>({Element}));
	
	IconBillboardComponent->SetHiddenInGame(true);
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

void AItem::ShowIcon(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	IconBillboardComponent->SetHiddenInGame(false);
}

void AItem::HideIcon(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IconBillboardComponent->SetHiddenInGame(true);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
