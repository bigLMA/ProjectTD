// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmourBreakingTurret.h"
#include "ArmourBreakingProjectile.h"

AArmourBreakingTurret::AArmourBreakingTurret()
	:ArmourBreakStrength(5), ArmourBreakDuration(1.4f)
{
}

void AArmourBreakingTurret::Upgrade(EUpgradeType UpgradeType)
{
	//TODO Uprgrades
}

void AArmourBreakingTurret::PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target)
{
	Super::PrepareProjectile(Projectile, Target);

	// Check if projectile is slow projectile
	if (auto SlowProjectile = Cast<AArmourBreakingProjectile>(Projectile))
	{
		// Pass slow properties to projectile
		SlowProjectile->SetArmourBreakProperties(ArmourBreakStrength, ArmourBreakDuration);
	}
}
