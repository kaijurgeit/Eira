// Copyright Epic Games, Inc. All Rights Reserved.

#include "EiraCharacter.h"

#include "EiraPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Eira/Game/EiraGameplayTags.h"
#include "Eira/Game/EiraInputComponent.h"
#include "AbilitySystem/EiraAttributeSet.h"
#include "AbilitySystem/ResourcesAttributeSet.h"
#include "AbilitySystem/EiraAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RadialMenu.h"


//////////////////////////////////////////////////////////////////////////
// AEiraCharacter

AEiraCharacter::AEiraCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionSphere = CreateDefaultSubobject<USphereComponent>("Interaction Sphere");
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetCollisionProfileName("Interact");

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	AbilitySystemComponent = CreateDefaultSubobject<UEiraAbilitySystemComponent>("AbilitySystemComponent");

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

UAbilitySystemComponent* AEiraCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEiraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
		
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveAbilities();
}

void AEiraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
		
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	GiveAbilities();
}

TArray<UShapeComponent*> AEiraCharacter::GetCollidersThatHaveTags_Implementation(FGameplayTagContainer ColliderTags)
{
	TArray<UShapeComponent*> Colliders = TArray<UShapeComponent*>();
	for (const TPair<FGameplayTag, UShapeComponent*> Pair : TagColliderMap)
	{
		if(ColliderTags.HasTag(Pair.Key))
		{
			if(IsValid(Pair.Value))
			{				
				Colliders.Add(Pair.Value);
				
				Pair.Value->SetHiddenInGame(false);
				Pair.Value->SetGenerateOverlapEvents(true);			
			}
		}
	}
	return Colliders;
}

UShapeComponent* AEiraCharacter::GetColliderThatHasTag_Implementation(FGameplayTag ColliderTag)
{
	TArray<UShapeComponent*> Colliders = TArray<UShapeComponent*>();
	for (const TPair<FGameplayTag, UShapeComponent*> Pair : TagColliderMap)
	{
		if(ColliderTag == Pair.Key)
		{
			return Pair.Value;
		}
	}
	return nullptr;
}

void AEiraCharacter::Equip(AItem* Item, FName SocketName)
{
	Unequip();
	EquippedItem = Item;
	AttachToSocket(Item, SocketName);
}

void AEiraCharacter::Unequip()
{
	if(!EquippedItem) { return; }
	InventoryComponent->Deselect(EquippedItem->StaticInventory[0].ItemDef);
	EquippedItem->Destroy();	
}

void AEiraCharacter::AttachToSocket(AItem* Item, FName SocketName)
{	
	const FAttachmentTransformRules AttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		false);
	Item->AttachToComponent(GetMesh(), AttachmentTransformRules, SocketName);
	UE_LOG(LogTemp, Warning, TEXT("%s, - Actor: %s, - SocketName: %s"),
		*FString(__FUNCTION__), *Item->GetName(), *SocketName.ToString());
	Item->Attach();
	Attachments.Add({SocketName, Item});
}

void AEiraCharacter::ClearSocket(FName SocketName)
{
	Attachments.FindAndRemoveChecked(SocketName)->Destroy();
}


void AEiraCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	Attributes = AbilitySystemComponent->AddSet<UEiraAttributeSet>();
	Resources = AbilitySystemComponent->AddSet<UResourcesAttributeSet>();

	//Add Input Mapping Context
	if (PlayerController = Cast<AEiraPlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	RadialMenu = Cast<URadialMenu>(CreateWidget(PlayerController, QuickInventoryMenuClass));
	RadialMenu->AddToViewport();
	RadialMenu->RemoveFromParent();
	
	FullMenu = Cast<UUserWidget>(CreateWidget(PlayerController, FullMenuClass));
	FullMenu->AddToViewport();
	FullMenu->RemoveFromParent();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEiraCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEiraInputComponent* EiraIC = CastChecked<UEiraInputComponent>(PlayerInputComponent))
	{
		const FEiraGameplayTags GameplayTags = FEiraGameplayTags::Get();

		TArray<uint32> BindHandles;
		EiraIC->BindAbilityActions(InputConfig, this, &AEiraCharacter::Input_AbilityInputTagPressed, &AEiraCharacter::Input_AbilityInputTagReleased, BindHandles);
		
		// Jumping
		EiraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Jump, ETriggerEvent::Triggered, this, &AEiraCharacter::Input_Jump);

		// Moving
		EiraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &AEiraCharacter::Move);

		// Looking
		EiraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look, ETriggerEvent::Triggered, this, &AEiraCharacter::Look);
		
		// Open/Close Quick Access Inventory
		EiraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_RadialMenu_Open, ETriggerEvent::Started, this, &AEiraCharacter::OpenRadialMenu);
		EiraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_RadialMenu_Close, ETriggerEvent::Completed, this, &AEiraCharacter::CloseRadialMenu);

		// Open/Close Main Menu
		EiraIC->BindNativeAction(InputConfig, GameplayTags.InputTag_FullMenu_OpenClose, ETriggerEvent::Started, this, &AEiraCharacter::OpenCloseFullMenu);
	}

}

void AEiraCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void AEiraCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}

void AEiraCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	Jump();
}

void AEiraCharacter::SetInteractableTargetActor(AActor* Value)
{
	InteractableTargetActor = Value;
}

AActor* AEiraCharacter::GetInteractableTargetActor()
{
	return InteractableTargetActor;
}

void AEiraCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEiraCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AEiraCharacter::GiveAbilities()
{
	if(HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& DefaultAbility : DefaultAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(DefaultAbility, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

void AEiraCharacter::OpenRadialMenu()
{
	RadialMenu->AddToViewport();
	PlayerController->SetInputMode(FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways));
	UGameplayStatics::SetGlobalTimeDilation(this, TimeDilation);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(QuickInventoryMappingContext, 1);
	}
}

void AEiraCharacter::CloseRadialMenu()
{
	UInventoryItemDefinition* ItemDef = RadialMenu->GetSelectedItemDef(); 
	InventoryComponent->Select(ItemDef);
	RadialMenu->RemoveFromParent();
	PlayerController->SetInputMode(FInputModeGameOnly());
	constexpr const float NormalTime = 1.0f;
	UGameplayStatics::SetGlobalTimeDilation(this, NormalTime);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(QuickInventoryMappingContext);
	}
}

void AEiraCharacter::OpenCloseFullMenu()
{
	if(!bIsFullMenuOpen)
	{
		OpenFullMenu();
	}
	else
	{
		CloseFullMenu();
	}	
	bIsFullMenuOpen = bIsFullMenuOpen ? false : true;
}

void AEiraCharacter::OpenFullMenu()
{
	FullMenu->AddToViewport();
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways));
	UGameplayStatics::SetGlobalTimeDilation(this, 0.f);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(FullMenuMappingContext, 1);
	}
}

void AEiraCharacter::CloseFullMenu()
{
	FullMenu->RemoveFromParent();
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
	constexpr const float NormalTime = 1.0f;
	UGameplayStatics::SetGlobalTimeDilation(this, NormalTime);
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(FullMenuMappingContext);
	}
}
