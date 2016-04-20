// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShips.h"
#include "ShipPawn.h"
#include "Enemy.h"


// Sets default values
AShipPawn::AShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetWorldRotation(FRotator(0, 270, 270));
	RootComponent = PlaneMesh;

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

	Acceleration = 500;
	TurnSpeed = 50;
	MaxSpeed = 4000;
	MinSpeed = 500;

	//Create the triggerSphere
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->AttachTo(RootComponent);
	TriggerSphere->SetSphereRadius(200.0f);

}

// Called when the game starts or when spawned
void AShipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShipPawn::Tick( float DeltaTime )
{
	const FVector LocalMove = FVector(0.0f, 0.0f, CurrentForwardSpeed * DeltaTime);

	// Move plan forwards (with sweep so we stop when we collide with things)
	AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);

	CurrentSpecialTimer -= DeltaTime;
	

	// Call any parent class Tick implementation
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Bind our control axis' to callback functions
	InputComponent->BindAxis("Thrust", this, &AShipPawn::ThrustInput);
	InputComponent->BindAxis("MoveRight", this, &AShipPawn::TurnInput);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AShipPawn::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AShipPawn::FireReleased);
	InputComponent->BindAction("Special", IE_Pressed, this, &AShipPawn::Special);
}

void AShipPawn::ThrustInput(float Val)
{
	// Is there no input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}


void AShipPawn::TurnInput(float Val)
{
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}


void AShipPawn::Special()
{
	if (CurrentSpecialTimer <= 0) {
		// Get all overlaping actors and store them in an array
		TArray<AActor*> OverlappingActors;
		TriggerSphere->GetOverlappingActors(OverlappingActors);
		UE_LOG(LogClass, Log, TEXT("Special!"));
		for (int32 iActor = 0; iActor < OverlappingActors.Num(); ++iActor)
		{
			UE_LOG(LogClass, Log, TEXT("hit %i"), iActor);
			// Cast the actor to a Pickup
			AEnemy* const TestEnemy = Cast<AEnemy>(OverlappingActors[iActor]);
			// If the cast is successfull and the pickup is valid and active
			if (TestEnemy && !TestEnemy->IsPendingKill() && TestEnemy->IsActive())
			{
				// Call the pickup`s WasCollected functions
				TestEnemy->TakeDamage(1);
				TestEnemy->TakeSpecial();
			}
		}
		CurrentSpecialTimer = SpecialCooldown;
	}
}

float AShipPawn::GetMaxPower()
{
	return MaxPower;
}

float AShipPawn::GetMinPower()
{
	return MinPower;
}

float AShipPawn::GetSpecialTimer()
{
	return 1 - (CurrentSpecialTimer / SpecialCooldown);
}

