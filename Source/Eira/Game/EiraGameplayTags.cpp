#include "EiraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FEiraGameplayTags FEiraGameplayTags::GameplayTags;

void FEiraGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FEiraGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	// Input
	AddTag(InputTag_Move, "InputTag.Move", "Move input.");
	AddTag(InputTag_Look, "InputTag.Look", "Look (mouse) input.");
	AddTag(InputTag_Jump, "InputTag.Jump", "Jump input");
	AddTag(InputTag_Fire, "InputTag.Fire", "Fire input.");
	AddTag(InputTag_RadialMenu_Open, "InputTag.RadialMenu.Open", "Open Radial Menu.");
	AddTag(InputTag_RadialMenu_Close, "InputTag.RadialMenu.Close", "Close Radial Menu.");
	AddTag(InputTag_FullMenu_OpenClose, "InputTag.FullMenu.OpenClose", "Full menu toggle open/close.");

	// Items
	// AddTag(ItemTag_StackCount_Max, "Item.StackCount.Max", "Max amount items of this type can be stacked. 0 Means not stackable");
	// AddTag(ItemTag_TotalCount_Max, "Item.TotalCount.Max", "Total count of items in this inventory. Can be modified (e.g. *1.5 or +10 through unlocking new inventory.");
	// AddTag(ItemTag_Inventory_Group, "Item.Inventory.Group", "Total count of items in this inventory. Can be modified (e.g. *1.5 or +10 through unlocking new inventory.");
}

void FEiraGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
