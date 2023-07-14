// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemFragments/InventoryItemFragment_QuickAccess.h"

#include "RadialMenu.generated.h"

class UImage;
class UCanvasPanel;
class USizeBox;
class UInventoryMenuSlot;
class UInventoryComponent;

UCLASS(Blueprintable)
class EIRA_API URadialMenu : public UUserWidget
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable)
	void SelectItemFromInventory();

	UFUNCTION(BlueprintCallable)
	void DropItemFromInventory();
	
protected:	
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintNativeEvent)
	void UpdateSectors(const TArray<FInventoryEntry>& Inventory);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "TheGame|UI", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> Menu;
	/** Size Boxes are the Sectors of the Radial Menu */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TheGame|UI")
	TArray<TObjectPtr<USizeBox>> SizeBoxSectors;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TheGame|UI")
	TArray<FSectorInfo> SectorInfos;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TheGame|UI")
	TArray<float> SectorAngles = { 45.f, 135.f, 225.f, 270.f, 315.f };
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TheGame|UI")
	float FullyCreatedDelay = .05f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TheGame|UI")
	FLinearColor HighlightColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TheGame|UI")
	FLinearColor NoColor = FLinearColor(1.f, 1.f, 1.f, 0.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TheGame|UI")
	FLinearColor DefaultColor = FLinearColor(1.f, 1.f, 1.f, .5f);	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TheGame|UI")
	float CountDistToCenter = 250.f;

private:
	int LastSectorIndex;
	FVector2d MenuCenterOnViewport;
	FTimerHandle TimerHandle;
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
	

	void InitializeDesign();
	void SetMenuCenterOnViewport();	
	void HighlightSector();
	float GetMouseAngle() const;
	int GetIndexByAngle(float MouseAngle) const;
	void GetChildImages(int SectorIndex, UImage*& BackgroundImage, UImage*& IconImage);
	URadialMenuSlot* GetInventorySlot(int SectorIndex);
	void Highlight(UImage* Image, bool IsHovered) const;
	void HighlightByIndex(float SectorIndex, bool IsHovered);
	void SetCountPositionByIndex(int i);
	// FSectorInfo* GetSectorInfo(FStorageInfo SlotInfo);
	FSectorInfo GetSectorInfoFromMouseAngle() const;
};