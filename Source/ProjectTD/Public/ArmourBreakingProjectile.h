// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretProjectile.h"
#include "ArmourBreakingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTTD_API AArmourBreakingProjectile : public ATurretProjectile
{
	GENERATED_BODY()
	
public:
	// Sets Armour break properties
	void SetArmourBreakProperties(int32 Strength, float Duration);

protected:
	// Called when hits
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	// Armour break properties
	int32 ArmourBreakStrength;
	float ArmourBreakDuration;
};
