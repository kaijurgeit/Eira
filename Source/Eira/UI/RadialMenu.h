// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/ItemFragments/InventoryItemFragment_RadialMenu.h"

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
	
	UFUNCTION(BlueprintNativeEvent)
	void SelectItem();
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:	
	UPROPERTY(BlueprintReadOnly, Category = "Eira|UI", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> Menu;
	
	/**
	 * Sectors
	 * - SizeBoxesSectors are the PanelWidgets that make the Sectors of the Radial Menu
	 * - SectorInfos associate Sectors with EStorageName
	 * - SectorAngles define the upper boundaries in degree of each Sector
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Eira|UI")
	TArray<TObjectPtr<USizeBox>> SizeBoxSectors;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Eira|UI")
	TArray<FSectorInfo> SectorInfos;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eira|UI")
	TArray<float> SectorAngles = { 45.f, 135.f, 225.f, 270.f, 315.f };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eira|UI")
	float FullyCreatedDelay = .05f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eira|UI")
	FLinearColor HighlightColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eira|UI")
	FLinearColor NoColor = FLinearColor(1.f, 1.f, 1.f, 0.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eira|UI")
	FLinearColor DefaultColor = FLinearColor(1.f, 1.f, 1.f, .5f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eira|UI")
	float CountDistToCenter = 250.f;	
	
	UPROPERTY(BlueprintReadWrite, Category = "Eira|Inventory")
	TObjectPtr<UInventoryItemDefinition> ItemDef;

private:	
	int LastSectorIndex;
	FVector2d MenuCenterOnViewport;
	FTimerHandle TimerHandle;	

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