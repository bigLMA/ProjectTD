// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "ProbeTurret.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API AProbeTurret : public ATurret
{
	GENERATED_BODY()
	
public:
	// Upgrade turret
	virtual void Upgrade(EUpgradeType UpgradeType) override;

protected:
	virtual void PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bounty increasment")
	int32 BountyIncreasment;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bounty increasment")
	float IncreasmentDuration;
};
