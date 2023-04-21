// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingComponent.h"
#include "Enemy.h"

UHealingComponent::UHealingComponent()
	:Regeneration(2), RegenInterval(0.5)
{
}

// Called when the game starts or when spawned
void UHealingComponent::BeginPlay()
{
	FTimerHandle RegenerationTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RegenerationTimerHandle, 
		this, &UHealingComponent::Regenerate, RegenInterval, true);
}

void UHealingComponent::Regenerate()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (auto Actor : OverlappingActors)
	{
		if (auto Enemy = Cast<AEnemy>(Actor))
		{
			Enemy->Regenerate(Regeneration);
		}
	}
}
