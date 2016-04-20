// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShips.h"
#include "Enemy.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// All pickups start active
	bIsActive = true;

	// Create the static mesh component
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	RootComponent = EnemyMesh;
	life = 5.0f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

bool AEnemy::IsActive()
{
	return bIsActive;
}

void AEnemy::SetActive(bool newPickupState)
{
	bIsActive = newPickupState;
}

void AEnemy::TakeDamage(float damage)
{
	life = life - damage;	
	FString EnemyDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have hit %s"), *EnemyDebugString);
	if (life <= 0)
	{
		UE_LOG(LogClass, Log, TEXT("You have killed %s"), *EnemyDebugString);
		Destroy();
	}

}