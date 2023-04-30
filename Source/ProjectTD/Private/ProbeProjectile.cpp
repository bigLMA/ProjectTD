// Fill out your copyright notice in the Description page of Project Settings.


#include "ProbeProjectile.h"
#include "Enemy.h"
#include "ShieldComponent.h"

void AProbeProjectile::SetBountyProperties(int32 Increasment, float Duration)
{
	BountyIncreasment = Increasment;
	EffectDuration = Duration;
}

void AProbeProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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
			Enemy->ApplyIncreasedMoney(BountyIncreasment, EffectDuration);
		}
	}

	Super::OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);
}
