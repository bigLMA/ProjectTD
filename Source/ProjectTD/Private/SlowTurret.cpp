// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowTurret.h"
#include "SlowProjectile.h"

ASlowTurret::ASlowTurret()
: SlowStrength(0.125f), SlowDuration(1.2f)
{
}

void ASlowTurret::Upgrade(EUpgradeType UpgradeType)
{
	Super::Upgrade(UpgradeType);

	int32 Index;

	CheckForUpgrade(UpgradeType, Index);

	switch (UpgradeType)
	{
	case EUpgradeType::Slow:
		SlowStrength = Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		break;
	default:
		break;
	}
}

void ASlowTurret::PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target)
{
	Super::PrepareProjectile(Projectile, Target);

	// Check if projectile is slow projectile
	if (auto SlowProjectile = Cast<ASlowProjectile>(Projectile))
	{
		// Pass slow properties to projectile
		SlowProjectile->SetSlowProperties(SlowStrength, SlowDuration);
	}
}
