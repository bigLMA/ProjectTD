// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "ArmourBreakingTurret.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API AArmourBreakingTurret : public ATurret
{
	GENERATED_BODY()

public:
	AArmourBreakingTurret();

	// Upgrade turret
	virtual void Upgrade(EUpgradeType UpgradeType) override;

protected:
	// Slow strength
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Armour break", meta = (ClampMin = 1, UIMin = 1, ClampMax = 50, ClampUI = 50))
	int32 ArmourBreakStrength;

	// Slow duration
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Armour break", meta = (ClampMin = 0.5f, UIMin = 0.5f, ClampMax = 5.f, ClampUI = 5.f))
	float ArmourBreakDuration;

	// Sets projectile variables, activates movement
	virtual void PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target) override;
};