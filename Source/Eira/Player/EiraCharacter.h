// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTags.h"
#include "InputActionValue.h"
#include "Interfaces/ColliderTagsInterface.h"
#include "Interfaces/InteractableSource.h"
#include "EiraCharacter.generated.h"

class UInventoryComponent;
class USphereComponent;
class UEiraAttributeSet;
class UResourcesAttributeSet;
class UEiraAbilitySystemComponent;
class UEiraInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGameplayAbility;
class AEiraPlayerController;
class URadialMenu;
class UEiraInventoryManagerComponent;
class AItem;

UCLASS(config=Game)
class AEiraCharacter : public ACharacter, public IAbilitySystemInterface, public IColliderTagsInterface, public IInteractableSource
{
	GENERATED_BODY()
 
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> QuickInventoryMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> FullMenuMappingContext;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEiraAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	const UEiraAttributeSet* Attributes;

	UPROPERTY()
	const UResourcesAttributeSet* Resources;

	UPROPERTY()
	TObjectPtr<AItem> EquippedItem;

public:
	AEiraCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual TArray<UShapeComponent*> GetCollidersThatHaveTags_Implementation(FGameplayTagContainer ColliderTags) override;	
	virtual UShapeComponent* GetColliderThatHasTag_Implementation(FGameplayTag ColliderTag) override;
	void Equip(AItem* Item, FName SocketName);
	void Unequip();
	void AttachToSocket(AItem* Item, FName SocketName);
	void ClearSocket(FName SocketName);
	
	/** Effect that initializes our default attributes. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

protected:	
	// To add mapping context
	virtual void BeginPlay();

	// Ssetup Input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

	/** Handles Jumping */
	void Input_Jump(const FInputActionValue& InputActionValue);
	
	virtual void SetInteractableTargetActor(AActor* Value) override;
	virtual AActor* GetInteractableTargetActor() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UEiraInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> QuickInventoryMenuClass;
	
	UPROPERTY()
	TObjectPtr<URadialMenu> RadialMenu;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> FullMenuClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> FullMenu;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeDilation = .25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, TObjectPtr<UShapeComponent>> TagColliderMap;
	
private:
	UPROPERTY()
	TObjectPtr<AEiraPlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<AActor> InteractableTargetActor;
	
	TMap<FName, TObjectPtr<AActor>> Attachments;

	bool bIsFullMenuOpen = false;
	
	virtual void GiveAbilities();
	void OpenRadialMenu();
	void CloseRadialMenu();
	void OpenCloseFullMenu();
	void OpenFullMenu();
	void CloseFullMenu();
};

