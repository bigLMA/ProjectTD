// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Path.h"
#include "AIController.h"

// Sets default values
AEnemy::AEnemy()
: Bounty(10), BountyBase(10), BountyDispersion(0), MaxHealth(50), DistanceThreshold(70)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Intialize actor components
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);

	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	Collision->SetupAttachment(Mesh);

	FloatingPawnComponent = CreateDefaultSubobject<UFloatingPawnMovement>("Pawn Movement");
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	WalkToNextTarget();

	InitializeEnemy();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::RecieveDamage(int32 Damage)
{
	Health -= Damage;

	if (Health <= 0)
	{
		Die();
	}
}

void AEnemy::ApplyBurning(int32 BurningDamage, float BurningDuration, float BurningInterval)
{
	int32 Index = -1;

	// Ñheck if enemy already has effect applied
	if (CheckForEffect(EEffects::Burning, Index))
	{
		// Renew effect timer

	}
	else
	{
		// Apply effect
	//	auto TakeBurningDamage = [&](int32 BurningDamage) {TakeDamageFromBurning(BurningDamage);

		GetWorld()->GetTimerManager().SetTimer(BurningTimerHandle, 
			this, &AEnemy::OnBurningDamageTimerFinished, BurningDuration, false);
		GetWorld()->GetTimerManager().SetTimer(BurningDamageTimerHandle, [this, BurningDamage]()
		{
				TakeDamageFromBurning(BurningDamage);
		}, BurningInterval, true);
	}
}

// Called when bunribg efferct finishes
void AEnemy::OnBurningDamageTimerFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(BurningTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BurningDamageTimerHandle);
}

// Called when enemy takes damage over time from burning
void AEnemy::TakeDamageFromBurning(int32 BurningDamage)
{
	RecieveDamage(BurningDamage);
}

// Apply slow on Enemy
void AEnemy::ApplySlow(float SlowStrength, float SlowDuration)
{
	int Index = -1;

	// Ñheck if enemy already has effect applied
	if (CheckForEffect(EEffects::Slow, Index))
	{
		// Renew effect timer
		GetWorld()->GetTimerManager().ClearTimer(SlowTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SlowTimerHandle,
			this, &AEnemy::RemoveSlow, SlowDuration, false);
	}
	else
	{
		Effects.Add(EEffects::Slow);

		// Otherwise apply effect
		FloatingPawnComponent->MaxSpeed *= (1 - SlowStrength);

		// Set timer to remove slow
		GetWorld()->GetTimerManager().SetTimer(SlowTimerHandle,
			this, &AEnemy::RemoveSlow, SlowDuration, false);
	}
}

// Removes slow from enemy
void AEnemy::RemoveSlow()
{
	Effects.RemoveSingle(EEffects::Slow);
	FloatingPawnComponent->MaxSpeed = InitialSpeed;
}

// Plays when enemy die
void AEnemy::Die()
{
	RewardPlayer();
	Destroy();
}

// Reward player with a bounty
void AEnemy::RewardPlayer()
{
	//TODO
}

// Checks if enemy has that effect
// Return -1 in index if false
bool AEnemy::CheckForEffect(EEffects Effect, int32& Index)
{
	return false;
}

// Sets all enemy variables, that cannot be set on construction
void AEnemy::InitializeEnemy()
{
	Health = MaxHealth;
	InitialSpeed = FloatingPawnComponent->MaxSpeed;
	RandomizeEnemyBounty();
}

// Called on BeginPlay to change bounty 
void AEnemy::RandomizeEnemyBounty()
{
	Bounty = FMath::RandRange(BountyBase - BountyDispersion, BountyBase + BountyDispersion);
}

// Called on BeginPlay, and when enemy is closed to target
void AEnemy::WalkToNextTarget()
{
	//if () { return; }

	TArray<AActor*> Paths;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APath::StaticClass(), Paths);

	if (auto Path = Cast<APath>(Paths[0]))
	{
		// Order AI to move to new location
		++TargetIndex;
		FVector TargetLocation;
		Path->GetLocationFromIndex(TargetIndex, TargetLocation);

		if (auto AIController = GetController<AAIController>())
		{
			AIController->MoveToLocation(TargetLocation);
			GetWorld()->GetTimerManager().SetTimer(TimerProximityTimerHandle,
				this, &AEnemy::CheckTargetProximity, 0.1f, true);
		}
	}
}

// Checks if enemy is closed to target
void AEnemy::CheckTargetProximity()
{
	TArray<AActor*> Paths;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APath::StaticClass(), Paths);

	if (auto Path = Cast<APath>(Paths[0]))
	{
		FVector TargetLocation;
		Path->GetLocationFromIndex(TargetIndex, TargetLocation);

		// Check if distance to target is less or equal then threshold
		auto VDistance = GetActorLocation() - TargetLocation;
		float Distance = VDistance.Size();
		
		if (Distance <= DistanceThreshold)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerProximityTimerHandle);
			// Order enemy to walk to next target
			WalkToNextTarget();
		}
	}
}