// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "SlowTurret.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API ASlowTurret : public ATurret
{
	GENERATED_BODY()
	
public:
	ASlowTurret();

	// Upgrade turret
	virtual void Upgrade(EUpgradeType UpgradeType) override;

protected:
	// Slow strength
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slow", meta = (ClampMin = 0.05f, UIMin = 0.05f, ClampMax = 0.9f, ClampUI = 0.9f))
	float SlowStrength;

	// Slow duration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slow", meta = (ClampMin = 0.5f, UIMin = 0.5f, ClampMax = 5.f, ClampUI = 5.f))
	float SlowDuration;

	// Sets projectile variables, activates movement
	virtual void PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target) override;
};
