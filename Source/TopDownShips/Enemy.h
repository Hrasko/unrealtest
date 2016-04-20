// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TOPDOWNSHIPS_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/*Return the mesh*/
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return EnemyMesh; }
	
	/* Return whether or not the pickup is active */
	UFUNCTION(BlueprintPure, Category = "Enemy")
	bool IsActive();

	/* Allows other classes to safely change the pickup activation */
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetActive(bool NewPickupState);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void TakeDamage(float damage);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
		void TakeSpecial();

protected:
	bool bIsActive;

private:
	UPROPERTY(VisibleAnywhere, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	float life;

	/*Static mesh to represent the pickup*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* EnemyMesh;

};
