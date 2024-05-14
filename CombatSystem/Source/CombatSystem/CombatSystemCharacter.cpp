// Copyright Epic Games, Inc. All Rights Reserved.

#include "CombatSystemCharacter.h"


#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components\SphereComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/Vector.h"
#include <Net\UnrealNetwork.h>
#include "Kismet/KismetSystemLibrary.h"


#include "Timer\Timer.h"
#include "PositionContorller.h"

//////////////////////////////////////////////////////////////////////////
// ACombatSystemCharacter

ACombatSystemCharacter::ACombatSystemCharacter()
{

	this->bReplicates = true;


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->bUseControllerDesiredRotation = false; // Character Rotates in the direction of the Camera
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)


	// Timer
	CombatTimerComp = CreateDefaultSubobject<UTimer>(TEXT("CombatTimerComp"));
	
	//PositionController
	PositionController = CreateDefaultSubobject<UPositionContorller>(TEXT("PositionController"));

	//PlayerHitBox
	PlayerHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerHitbox"));
	PlayerHitbox->SetupAttachment(GetRootComponent());
	PlayerHitbox->SetRelativeScale3D(FVector((4.5f, 7.f, 3.75f)));

}

void ACombatSystemCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(150, 10, FColor::Green, FString(TEXT("Begin Play")));
	}*/

	/*headTrackingSettings.Speed = 1;
	headTrackingSettings.offset = FVector(1, 1, 1);*/

	//Add Input Mapping Context
	/*if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}*/

}


void ACombatSystemCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME_CONDITION(ACombatSystemCharacter, PositionController, COND_AutonomousOnly);

	DOREPLIFETIME_CONDITION(ACombatSystemCharacter, movementState, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemCharacter, combatState, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemCharacter, attackPattern, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemCharacter, attackLethality, COND_AutonomousOnly);

	DOREPLIFETIME_CONDITION(ACombatSystemCharacter, bIsSprinting, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemCharacter, CurrentCombo, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(ACombatSystemCharacter, bIsTryingToBlock, COND_AutonomousOnly);



	//DOREPLIFETIME_CONDITION(ACombatSystemCharacter, ACombatSystemCharacter::GetCharacterMovement()->bUseControllerDesiredRotation, COND_AutonomousOnly);

}






//////////////////////////////////////////////////////////////////////////
// Input

void ACombatSystemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	//Super::SetupPlayerInputComponent(PlayerInputComponent);


	//if (GEngine) {
	//	GEngine->AddOnScreenDebugMessage(150, 10, FColor::Green, FString(TEXT("Inputs")));
	//}


	//// Set up action bindings
	//if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
	//	
	//	//Jumping
	//	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	//	//Moving
	//	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatSystemCharacter::Move);

	//	//Looking
	//	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatSystemCharacter::Look);

	//}

}


void ACombatSystemCharacter::Move(const FInputActionValue& Value)
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

void ACombatSystemCharacter::Look(const FInputActionValue& Value)
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


//-------------------------------------------
//-------------------------------------------
//-------------------------------------------
//-------------------------------------------




void ACombatSystemCharacter::UpdateState(FStateData Data)
{

	if (UCharacterMovementComponent* movementComp = GetCharacterMovement())
	{
		movementComp->MaxWalkSpeed = Data.WalkSpeed;
		movementComp->MaxAcceleration = Data.Acceleration;
		movementComp->BrakingDecelerationWalking = Data.Deceleration;
	}
}

void ACombatSystemCharacter::Client_ScanForLockOnTarget_Implementation()
{
	//Fire a ray from the camera and look for a tag named player

	if (!GEngine) { return; }

	if (UWorld* world = GetWorld()) {

		FVector CamLocation = GetFollowCamera()->GetComponentLocation();
		FVector Direction = (CamLocation + (CamLocation.ForwardVector * FVector(1000)));

		UKismetSystemLibrary::DrawDebugLine(world, CamLocation, Direction, FLinearColor::Red, 1.f, 5.f);

		GEngine->AddOnScreenDebugMessage(200, 10, FColor::Green, Direction.ToString());

		FHitResult result(ForceInit);
		world->LineTraceSingleByChannel(result, CamLocation, Direction, ECollisionChannel::ECC_Pawn);

		if (AActor* HitActor = result.GetActor()) {

			GEngine->AddOnScreenDebugMessage(100, 10, FColor::Red, HitActor->GetName());

		}
	}
}