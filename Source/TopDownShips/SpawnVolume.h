// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class TOPDOWNSHIPS_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/*Returns the WhereToSpawn subobject*/
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	/* Find a random point within the boxcomponent*/
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();	

protected:
	/*the pickup to spawn*/
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AEnemy> WhatToSpawn;

	FTimerHandle SpawnTimer;

	/* Min spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeLow;

	/* Max spawn delay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayRangeHigh;

private:
	/* Box component to specific where pickups should spawn */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta=(AllowPrivateAccess="true"))
	class UBoxComponent* WhereToSpawn;
	
	/*Handle spawning pickup*/
	void SpawnPickup();

	/* The current spawn delay */
	float SpawnDelay;
};
