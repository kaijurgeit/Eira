// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenu.h"

#include "InventoryMenuSlot.h"
#include "RadialMenuSlot.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/EiraCharacter.h"


void URadialMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(const AEiraCharacter* PlayerCharacter = Cast<AEiraCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(PlayerCharacter->GetComponentByClass(UInventoryComponent::StaticClass()));
		if(InventoryComponent)
		{
			InventoryComponent->UpdateInventory.AddUniqueDynamic(this, &URadialMenu::UpdateSectors);
		}		
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,
		&URadialMenu::InitializeDesign, FullyCreatedDelay, false);
}

void URadialMenu::SelectItem_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}

UInventoryItemDefinition* URadialMenu::GetSelectedItemDef()
{
	return Slots[SelectedSectorIndex]->ItemDef;
}

void URadialMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);	
	SelectedSectorIndex = GetIndexByAngle(GetMouseAngle());
	HighlightSector();	
}


void URadialMenu::UpdateSectors_Implementation(const TArray<FInventoryEntry>& InventoryEntries)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
}

void URadialMenu::SelectItemFromInventory()
{
}

void URadialMenu::DropItemFromInventory()
{
}

void URadialMenu::InitializeDesign()
{
	SetMenuCenterOnViewport();
	GetOwningPlayer()->SetMouseLocation(MenuCenterOnViewport.X, MenuCenterOnViewport.Y);
	for (int i = 0; i < SectorAngles.Num(); ++i)
	{
		SetCountPositionByIndex(i);
	}
}

void URadialMenu::SetMenuCenterOnViewport()
{
	FVector2d PixelPosition;
	FVector2d ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(this, Menu->GetCachedGeometry(),
		FVector2d(0.f, 0.f), PixelPosition, ViewportPosition);
	const FVector2d MenuSize = Menu->GetCachedGeometry().GetAbsoluteSize();
	MenuCenterOnViewport = (MenuSize / 2) + PixelPosition;
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta,
			FString::Printf(TEXT("%f, %f"), MenuCenterOnViewport.X, MenuCenterOnViewport.Y));
	}
	TimerHandle.Invalidate();
}

float URadialMenu::GetMouseAngle() const
{	
	float MouseX;
	float MouseY;
	APlayerController* PlayerController = GetOwningPlayer();
	if(!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
		return 0.f;
	}
	GetOwningPlayer()->GetMousePosition(MouseX, MouseY);
	const FVector Start = FVector(MouseX, MouseY, 0.f);
	const FVector End = FVector(MenuCenterOnViewport.X, MenuCenterOnViewport.Y, 0.f);
	const float Yaw = UKismetMathLibrary::FindLookAtRotation(Start, End).Yaw;
	return 180.0 -  Yaw;
}

int URadialMenu::GetIndexByAngle(const float MouseAngle)  const
{
		for (int i = 0; i < SectorAngles.Num(); ++i)
	{
		if(SectorAngles[i] > MouseAngle)
		{
			return i;
		}
	}
	return 0;
}

void URadialMenu::GetChildImages(int SectorIndex, UImage*& BackgroundImage, UImage*& IconImage)
{
	const UOverlay* SizeBoxOverlay = Cast<UOverlay>(SizeBoxSectors[SectorIndex]->GetChildAt(0));
	if(!SizeBoxOverlay)
	{
		return;
	}
	BackgroundImage = Cast<UImage>(SizeBoxOverlay->GetChildAt(0));
	const USizeBox* OverlaySizeBox = Cast<USizeBox>(SizeBoxOverlay->GetChildAt(1));
	IconImage = Cast<UImage>(OverlaySizeBox->GetChildAt(0));
	if(!IconImage)
	{
		if(const URadialMenuSlot* InventorySlot = Slots[SectorIndex])
		{
			IconImage = InventorySlot->Icon;
		}
	}		
}


void URadialMenu::Highlight(UImage* Image, bool IsHovered) const
{
	if(IsHovered)
	{
		Image->SetColorAndOpacity(HighlightColor);
	}
	else
	{
		Image->SetColorAndOpacity(DefaultColor);
	}
}

void URadialMenu::HighlightByIndex(float SectorIndex, bool IsHovered)
{
	UImage* BackgroundImage;
	UImage* IconImage;
	GetChildImages(SectorIndex, BackgroundImage, IconImage);
	if(!BackgroundImage || !IconImage)
	{
		return;
	}
	Highlight(BackgroundImage, IsHovered);	
	Highlight(IconImage, IsHovered);
}



void URadialMenu::SetCountPositionByIndex(int i)
{
	const float LowerAngle = (i == 0) ? 0.f : SectorAngles[i-1];
	const float UpperAngle = SectorAngles[i];
	const float MiddleAngle = (LowerAngle + UpperAngle) / 2.f;
	const double X = UKismetMathLibrary::Cos(FMath::DegreesToRadians(MiddleAngle));
	const double Y = -UKismetMathLibrary::Sin(FMath::DegreesToRadians(MiddleAngle));
	FVector2d CountPosition =  FVector2d(X, Y);
	CountPosition.Normalize();
	CountPosition = CountDistToCenter * CountPosition;
	UCanvasPanelSlot* Count = UWidgetLayoutLibrary::SlotAsCanvasSlot(Slots[i]->CountText);
	if(Count)
	{
		Count->SetPosition(CountPosition);
	}
}

void URadialMenu::HighlightSector()
{
	if(LastSectorIndex != SelectedSectorIndex)
	{		
		HighlightByIndex(LastSectorIndex, false);
		HighlightByIndex(SelectedSectorIndex, true);
		LastSectorIndex = SelectedSectorIndex;		
		UE_LOG(LogTemp, Warning, TEXT("%s -> %i"), *FString(__FUNCTION__), SelectedSectorIndex);
	}
}
