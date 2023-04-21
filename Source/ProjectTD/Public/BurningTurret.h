// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "BurningTurret.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API ABurningTurret : public ATurret
{
	GENERATED_BODY()
	
public:
	ABurningTurret();

	// Upgrade turret
	virtual void Upgrade(EUpgradeType UpgradeType) override;

protected:
	// Burning damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buring", meta = (ClampMin = 1, UIMin = 1))
	int32 BurningDamage;

	// Burning duration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buring", meta = (ClampMin = 0.5f, UIMin = 0.5f, ClampMax = 5.f, ClampUI = 5.f))
	float BurningDuration;

	// Burning interval
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buring", meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 0.8f, ClampUI = 0.8f))
	float BurningInterval;

	// Sets projectile variables, activates movement
	virtual void PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target) override;
};
