// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldComponent.h"

// Sets default values for this actor's properties
UShieldComponent::UShieldComponent()
	:MaxHealth(25), ShieldRegenerationRate(0.25f)
{
	PrimaryComponentTick.bCanEverTick = false;
}

// When shield takes damage
void UShieldComponent::RecieveDamage(int32 Damage)
{
	Health -= static_cast<float>(Damage);

	if (Health <= 0)
	{
		DestroyComponent();
	}
}

// Set shieled regeneration
void UShieldComponent::SetShieldRegeneration()
{
	Health = MaxHealth;
	FTimerHandle RegenerationTimerHandle;
	float RechargeRate = 0.05f;
	GetWorld()->GetTimerManager().SetTimer(RegenerationTimerHandle, [this, RechargeRate]()
		{
			RegenerateShield(RechargeRate);
		}, RechargeRate, true);
}

// Regenerates shield
void UShieldComponent::RegenerateShield(float DeltaTime)
{
	if (Health < MaxHealth)
	{
		Health += (ShieldRegenerationRate * DeltaTime);
	}
}
