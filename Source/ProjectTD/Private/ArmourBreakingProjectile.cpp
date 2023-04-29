// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmourBreakingProjectile.h"
#include "Enemy.h"
#include "ShieldComponent.h"

// Sets Armour break properties
void AArmourBreakingProjectile::SetArmourBreakProperties(int32 Strength, float Duration)
{
	 ArmourBreakStrength = Strength;
	 ArmourBreakDuration = Duration;
}

// Called when hits
void AArmourBreakingProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
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
		Enemy->ApplyArmourBreak(ArmourBreakStrength, ArmourBreakDuration);
	}

	Super::OnHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);
}
