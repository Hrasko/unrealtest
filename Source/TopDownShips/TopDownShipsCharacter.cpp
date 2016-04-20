// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TopDownShips.h"
#include "TopDownShipsCharacter.h"
#include "Enemy.h"

ATopDownShipsCharacter::ATopDownShipsCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create the triggerSphere
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->AttachTo(RootComponent);
	TriggerSphere->SetSphereRadius(200.0f);

	PrimaryActorTick.bCanEverTick = true;
}

void ATopDownShipsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Get all overlaping actors and store them in an array
	TArray<AActor*> OverlappingActors;
	TriggerSphere->GetOverlappingActors(OverlappingActors);
	for (int32 iActor = 0; iActor < OverlappingActors.Num(); ++iActor)
	{
		// Cast the actor to a Pickup
		AEnemy* const TestEnemy = Cast<AEnemy>(OverlappingActors[iActor]);
		// If the cast is successfull and the pickup is valid and active
		if (TestEnemy && !TestEnemy->IsPendingKill() && TestEnemy->IsActive())
		{
			// Call the pickup`s WasCollected functions
			TestEnemy->TakeDamage(DeltaTime);
		}
	}
}

void ATopDownShipsCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ATopDownShipsCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATopDownShipsCharacter::MoveRight);
}

void ATopDownShipsCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATopDownShipsCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
