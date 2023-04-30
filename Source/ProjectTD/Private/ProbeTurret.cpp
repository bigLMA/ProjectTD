// Fill out your copyright notice in the Description page of Project Settings.


#include "ProbeTurret.h"
#include "ProbeProjectile.h"

void AProbeTurret::Upgrade(EUpgradeType UpgradeType)
{
	Super::Upgrade(UpgradeType);

	int32 Index;

	CheckForUpgrade(UpgradeType, Index);

	switch (UpgradeType)
	{
	case EUpgradeType::Bounty_Increasment:
		BountyIncreasment = Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		break;
	default:
		break;
	}
}

void AProbeTurret::PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target)
{
	Super::PrepareProjectile(Projectile, Target);

	if (auto ProbeProjectile = Cast<AProbeProjectile>(Projectile))
	{
		ProbeProjectile->SetBountyProperties(BountyIncreasment, IncreasmentDuration);
	}
}
