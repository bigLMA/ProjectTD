// Fill out your copyright notice in the Description page of Project Settings.


#include "BurningTurret.h"
#include "BurningProjectile.h"

ABurningTurret::ABurningTurret()
	:BurningDamage(1), BurningDuration(2.f), BurningInterval(0.8f)
{
}

void ABurningTurret::Upgrade(EUpgradeType UpgradeType)
{
	//TODO Upgrade
}

void ABurningTurret::PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target)
{
	Super::PrepareProjectile(Projectile, Target);

	// Check if projectile is slow projectile
	if (auto SlowProjectile = Cast<ABurningProjectile>(Projectile))
	{
		// Pass slow properties to projectile
		SlowProjectile->SetBurningProperties(BurningDamage, BurningDuration, BurningInterval);
	}
}
