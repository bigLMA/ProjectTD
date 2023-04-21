// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretProjectile.h"
#include "SlowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API ASlowProjectile : public ATurretProjectile
{
	GENERATED_BODY()

public:
	void SetSlowProperties(float Strength, float Duration);

protected:
	// Called when hits
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	// Slow strength
	float SlowStrength;

	// Slow duration
	float SlowDuration;
};
