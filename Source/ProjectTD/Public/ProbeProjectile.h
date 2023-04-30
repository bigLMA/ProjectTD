// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretProjectile.h"
#include "ProbeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API AProbeProjectile : public ATurretProjectile
{
	GENERATED_BODY()
	
public:
	void SetBountyProperties(int32 Increasment, float Duration);

	// Called when hits
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	int32 BountyIncreasment;
	float EffectDuration;
};
