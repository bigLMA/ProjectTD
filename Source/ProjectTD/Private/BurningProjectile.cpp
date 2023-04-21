// Fill out your copyright notice in the Description page of Project Settings.


#include "BurningProjectile.h"
#include "Enemy.h"

void ABurningProjectile::SetBurningProperties(int32 BDamage, float BDuration, float BInterval)
{
	BurningInterval = BInterval;
	BurningDamage = BDamage;
	BurningDuration = BDuration;
}

void ABurningProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (auto Enemy = Cast<AEnemy>(OtherActor))
	{
		// Apply burning on enemy
		Enemy->ApplyBurning(BurningDamage, BurningDuration, BurningInterval);
	}

	Super::OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);
}
