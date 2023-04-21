// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowProjectile.h"
#include "Enemy.h"

void ASlowProjectile::SetSlowProperties(float Strength, float Duration)
{
	SlowStrength = Strength;
	SlowDuration = Duration;
}

// Called when hits
void ASlowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (auto Enemy = Cast<AEnemy>(OtherActor))
	{
		Enemy->ApplySlow(SlowStrength, SlowDuration);
	}

	Super::OnHit(HitComponent, OtherActor,OtherComponent, NormalImpulse, Hit);
}
