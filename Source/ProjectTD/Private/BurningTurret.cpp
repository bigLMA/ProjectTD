// Fill out your copyright notice in the Description page of Project Settings.


#include "BurningTurret.h"
#include "BurningProjectile.h"

ABurningTurret::ABurningTurret()
	:BurningDamage(1), BurningDuration(2.f), BurningInterval(0.8f)
{
}

void ABurningTurret::Upgrade(EUpgradeType UpgradeType)
{
	int32 Index;

	CheckForUpgrade(UpgradeType, Index);

	switch (UpgradeType)
	{
	case EUpgradeType::BurningDamage:
		BurningDamage= Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		
		break;
	default:
		break;
	}

	Super::Upgrade(UpgradeType);
}

void ABurningTurret::PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target)
{
	Super::PrepareProjectile(Projectile, Target);

	// Check if projectile is burning projectile
	if (auto BurningProjectile = Cast<ABurningProjectile>(Projectile))
	{
		// Pass burning properties to projectile
		BurningProjectile->SetBurningProperties(BurningDamage, BurningDuration, BurningInterval);
	}
}
