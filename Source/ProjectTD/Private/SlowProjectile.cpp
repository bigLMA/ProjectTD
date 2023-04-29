// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowProjectile.h"
#include "Enemy.h"
#include "ShieldComponent.h"

void ASlowProjectile::SetSlowProperties(float Strength, float Duration)
{
	SlowStrength = Strength;
	SlowDuration = Duration;
}

// Called when hits
void ASlowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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
		// Apply slow on enemy
		Enemy->ApplySlow(SlowStrength, SlowDuration);
	}

	Super::OnHit(HitComponent, OtherActor,OtherComponent, NormalImpulse, Hit);
}
