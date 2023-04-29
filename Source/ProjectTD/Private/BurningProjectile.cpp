// Fill out your copyright notice in the Description page of Project Settings.


#include "BurningProjectile.h"
#include "Enemy.h"
#include "ShieldComponent.h"

void ABurningProjectile::SetBurningProperties(int32 BDamage, float BDuration, float BInterval)
{
	BurningInterval = BInterval;
	BurningDamage = BDamage;
	BurningDuration = BDuration;
}

void ABurningProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (auto Shield = Cast<UShieldComponent>(OtherComponent))
	{
		// Calculate damage to shield
		float FDamageToShield = static_cast<float>(Damage) * DamageToShields;
		int32 DamageToShield = static_cast<int32>(FDamageToShield);

		// Damage shield
		Shield->RecieveDamage(DamageToShield);

		// Destroy projectile
		Destroy();
		return;
	}

	if (auto Enemy = Cast<AEnemy>(OtherActor))
	{
		if (!IsValid(Enemy)) { return; }
		{
			// Apply burning on enemy
			Enemy->ApplyBurning(BurningDamage, BurningDuration, BurningInterval);
		}
	}

	Super::OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);
}
