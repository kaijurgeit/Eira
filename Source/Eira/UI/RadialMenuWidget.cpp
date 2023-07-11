// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenuWidget.h"

#include "InventorySlot.h"
#include "QuickAccessSlot.h"
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

UE_DISABLE_OPTIMIZATION

void URadialMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(const AEiraCharacter* PlayerCharacter = Cast<AEiraCharacter>(GetOwningPlayer()->GetCharacter()))
	{
		InventoryComponent = Cast<UInventoryComponent>(PlayerCharacter->GetComponentByClass(UInventoryComponent::StaticClass()));
		if(InventoryComponent)
		{
			// InventoryComponent->UpdateStorage.AddUniqueDynamic(this, &URadialMenuWidget::UpdateSector);
		}		
	}
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,
		&URadialMenuWidget::InitializeDesign, FullyCreatedDelay, false);
}

void URadialMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	HighlightSector();	
}

void URadialMenuWidget::InitializeDesign()
{
	SetMenuCenterOnViewport();
	GetOwningPlayer()->SetMouseLocation(MenuCenterOnViewport.X, MenuCenterOnViewport.Y);
	for (int i = 0; i < SectorAngles.Num(); ++i)
	{
		SetCountPositionByIndex(i);
		SectorInfos[i].Sector = GetInventorySlot(i);
	}
}

void URadialMenuWidget::UpdateSector_Implementation(const TArray<FInventoryEntry>& Inventory)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	// const FSectorInfo* SectorInfo = GetSectorInfo(SlotInfo);
	// if(!SectorInfo)
	// {
	// 	return;
	// }
	// UpdateIcon(SlotInfo, *SectorInfo);
	// UpdateCount(SlotInfo, *SectorInfo);
}

// void URadialMenuWidget::UpdateCount(FStorageInfo SlotInfo, const FSectorInfo& SectorInfo) const
// {
// 	if(UTextBlock* TextBlock = Cast<UTextBlock>(SectorInfo.Sector->Count))
// 	{
// 		FString String;
// 		if(SlotInfo.Count == 0 || SlotInfo.Max < 2)
// 		{
// 			String = FString("");
// 		}
// 		else
// 		{
// 			String = FString::Printf(TEXT("%d/%d"), SlotInfo.Count, SlotInfo.Max);						
// 		}
// 		const FText Text = FText::FromString(String);
// 		TextBlock->SetText(Text);
// 		TextBlock->SetColorAndOpacity(DefaultColor);
// 	}
// }
//
// void URadialMenuWidget::UpdateIcon(FStorageInfo SlotInfo, const FSectorInfo& SectorInfo) const
// {
// 	// If no item in Slot remove Icon
// 	if(SlotInfo.Count == 0)
// 	{
// 		SectorInfo.Sector->Icon->Brush = FSlateBrush();			
// 		SectorInfo.Sector->Icon->SetBrushTintColor(NoColor);
// 		return;
// 	}
// 	if(SlotInfo.Icon)
// 	{
// 		SectorInfo.Sector->Icon->SetBrushFromTexture(SlotInfo.Icon);				
// 		SectorInfo.Sector->Icon->SetBrushTintColor(HighlightColor);		
// 	}
// }

void URadialMenuWidget::SelectItemFromInventory()
{
	const FSectorInfo SectorInfo = GetSectorInfoFromMouseAngle();
	// InventoryComponent->Select(SectorInfo.StorageName, SectorInfo.Slot);
}

void URadialMenuWidget::DropItemFromInventory()
{
	const FSectorInfo SectorInfo = GetSectorInfoFromMouseAngle();
	// InventoryComponent->Drop(SectorInfo.StorageName, SectorInfo.Slot);
}

void URadialMenuWidget::SetMenuCenterOnViewport()
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

float URadialMenuWidget::GetMouseAngle() const
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

int URadialMenuWidget::GetIndexByAngle(const float MouseAngle)  const
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

void URadialMenuWidget::GetChildImages(int SectorIndex, UImage*& BackgroundImage, UImage*& IconImage)
{
	const UOverlay* SizeBoxOverlay = Cast<UOverlay>(SectorInfos[SectorIndex].SizeBox->GetChildAt(0));
	if(!SizeBoxOverlay)
	{
		return;
	}
	BackgroundImage = Cast<UImage>(SizeBoxOverlay->GetChildAt(0));
	const USizeBox* OverlaySizeBox = Cast<USizeBox>(SizeBoxOverlay->GetChildAt(1));
	IconImage = Cast<UImage>(OverlaySizeBox->GetChildAt(0));
	if(!IconImage)
	{
		if(const UQuickAccessSlot* InventorySlot = GetInventorySlot(SectorIndex))
		{
			IconImage = InventorySlot->Icon;
		}
	}		
}

UQuickAccessSlot* URadialMenuWidget::GetInventorySlot(const int SectorIndex)
{
	const UOverlay* SizeBoxOverlay = Cast<UOverlay>(SectorInfos[SectorIndex].SizeBox->GetChildAt(0));
	if(!SizeBoxOverlay)
	{
		return nullptr;
	}
	const USizeBox* OverlaySizeBox = Cast<USizeBox>(SizeBoxOverlay->GetChildAt(1));
	return Cast<UQuickAccessSlot>(OverlaySizeBox->GetChildAt(0));
}

void URadialMenuWidget::Highlight(UImage* Image, bool IsHovered) const
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

void URadialMenuWidget::HighlightByIndex(float SectorIndex, bool IsHovered)
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

void URadialMenuWidget::HighlightSector()
{
	const int SectorIndex = GetIndexByAngle(GetMouseAngle());
	if(LastSectorIndex != SectorIndex)
	{		
		HighlightByIndex(LastSectorIndex, false);
		HighlightByIndex(SectorIndex, true);
		LastSectorIndex = SectorIndex;		
		UE_LOG(LogTemp, Warning, TEXT("%s -> %i"), *FString(__FUNCTION__), SectorIndex);
	}
}


void URadialMenuWidget::SetCountPositionByIndex(int i)
{
	const float LowerAngle = (i == 0) ? 0.f : SectorAngles[i-1];
	const float UpperAngle = SectorAngles[i];
	const float MiddleAngle = (LowerAngle + UpperAngle) / 2.f;
	const double X = UKismetMathLibrary::Cos(FMath::DegreesToRadians(MiddleAngle));
	const double Y = -UKismetMathLibrary::Sin(FMath::DegreesToRadians(MiddleAngle));
	FVector2d CountPosition =  FVector2d(X, Y);
	CountPosition.Normalize();
	CountPosition = CountDistToCenter * CountPosition;
	if(const UQuickAccessSlot* InventorySlot = GetInventorySlot(i))
	{
		// UCanvasPanelSlot* Count = Cast<UCanvasPanelSlot>(InventorySlot->Count);
		UCanvasPanelSlot* Count = UWidgetLayoutLibrary::SlotAsCanvasSlot(InventorySlot->Count);
		if(Count)
		{
			Count->SetPosition(CountPosition);
		}
	}
}

// FSectorInfo* URadialMenuWidget::GetSectorInfo(FStorageInfo SlotInfo)
// {
// 	for (FSectorInfo& SectorInfo : SectorInfos)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("StorageName: %d == %d"), SectorInfo.StorageName, SlotInfo.StorageName);
// 		if(SectorInfo.StorageName == SlotInfo.StorageName)
// 		{
// 			if(SectorInfo.Slot == SlotInfo.Index)
// 			{
// 				return &SectorInfo;
// 			}			
// 		}
// 	}
// 	return nullptr;
// }

FSectorInfo URadialMenuWidget::GetSectorInfoFromMouseAngle() const
{
	return SectorInfos[GetIndexByAngle(GetMouseAngle())];
}
