// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "ShipPawn.generated.h"

UCLASS()
class TOPDOWNSHIPS_API AShipPawn : public APawn
{
	GENERATED_BODY()
	/** StaticMesh component that will be the visuals for our ship */
	UPROPERTY(Category = "Plane", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** TriggerSphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* TriggerSphere;

public:
	// Sets default values for this pawn's properties
	AShipPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns PlaneMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }

	/** Bound to the thrust axis */
	void ThrustInput(float Val);

	/** Bound to the horizontal axis */
	void TurnInput(float Val);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ship")
	void FirePressed();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ship")
	void FireReleased();

	UFUNCTION(BlueprintPure, Category = "Ship")
		float GetMaxPower();

	UFUNCTION(BlueprintPure, Category = "Ship")
		float GetMinPower();

	UFUNCTION(BlueprintPure, Category = "Ship")
		float GetSpecialTimer();

protected:
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void Special();

private:

	/** How quickly forward speed changes */
	UPROPERTY(Category = "Plane", EditAnywhere)
		float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = "Plane", EditAnywhere)
		float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = "Plane", EditAnywhere)
		float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = "Plane", EditAnywhere)
		float MinSpeed;

	/** Max laser power */
	UPROPERTY(Category = "Plane", EditAnywhere)
		float MaxPower;

	/** Min laser power */
	UPROPERTY(Category = "Plane", EditAnywhere)
		float MinPower;

	/** Special timeout */
	UPROPERTY(Category = "Plane", EditAnywhere)
		float SpecialCooldown;

	float CurrentSpecialTimer;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;	

};
