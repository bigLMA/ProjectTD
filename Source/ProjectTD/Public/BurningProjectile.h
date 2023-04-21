// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretProjectile.h"
#include "BurningProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API ABurningProjectile : public ATurretProjectile
{
	GENERATED_BODY()
public:
	// Sets burning properties
	void SetBurningProperties(int32 BDamage, float BDuration, float BInterval);

protected:
	// Called when hits
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	// Burning properties
	int32 BurningDamage;
	float BurningDuration;
	float BurningInterval;
};
