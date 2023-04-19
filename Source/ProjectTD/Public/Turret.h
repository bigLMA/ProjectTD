// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

UCLASS()
class PROJECTTD_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Fire with at target with projectile
	virtual void Fire();

	// Aim at special location
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void AimAt(const AActor* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Cost of turret to build
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost", meta = (ClampMin = 1, UIMin = 1))
	int32 Cost;

	// Damage of turret
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = 1, UIMin = 1))
	int32 Damage;

	// Speed of firing new projectiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire rate", meta = (ClampMin = 0.1, UIMin = 0.1))
	float ReloadTime;

	//TODO Projectile class

	// Turret rotation per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret rotation")
	float RotationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	USceneComponent* Root;

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	UStaticMeshComponent* Tower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	UStaticMeshComponent* Base;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shoot Colision")
	//class USphereComponent* ShootColision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shoot Decal")
	UDecalComponent* ShootDecal;

private:	
	//Target of the turret
	//class AEnemy* Target;

	// Aiming timer
	FTimerHandle AimingTimerHandle;

	// Locks on target
	void LockOn(const AActor* Target, float Time);

	void Fire(const AActor* Target);

	double LastFireTime;
};
