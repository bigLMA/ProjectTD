// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Path.h"
#include "AIController.h"
#include "CameraPlayer.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthWidget.h"

// Sets default values
AEnemy::AEnemy()
: Bounty(10), BountyBase(10), BountyDispersion(0), MaxHealth(50), InitialArmour(10), DistanceThreshold(130), DamageToBase(7)
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
	FloatingPawnComponent->MaxSpeed = 300;

	HealthComponent = CreateDefaultSubobject<UWidgetComponent>("Health Component");
	HealthComponent->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	WalkToNextTarget();
	InitializeEnemy();

	if (auto HealthWidget = Cast<UEnemyHealthWidget>(HealthComponent->GetUserWidgetObject()))
	{
		HealthWidget->SetOwner(this);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when enemy takes damage
void AEnemy::ReceiveDamage(int32 Damage)
{
	// Check damage against armour, 
	// leave function if damage dealt would be less than 1
	float DamageDealt = static_cast<float>(Damage) *(1 - static_cast<float>(Armour/100));
	if (DamageDealt < 0) 
	{ 
		DamageDealt = -DamageDealt;
	}

	Health -= DamageDealt;

	if (Health <= 0)
	{
		Die();
	}
}

// Called when enemy is set on fire
void AEnemy::ApplyBurning(int32 BurningDamage, float BurningDuration, float BurningInterval)
{
	int32 Index = -1;

	if (CheckForImmunity(EEffects::Burning, Index))
	{
		return;
	}

	// Ñheck if enemy already has effect applied
	if (CheckForEffect(EEffects::Burning, Index))
	{
		// Renew effect timer
		GetWorld()->GetTimerManager().ClearTimer(BurningTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(BurningTimerHandle,
			this, &AEnemy::OnBurningDamageTimerFinished, BurningDuration, false);
	}
	else
	{
		// Apply effect
		Effects.Add(EEffects::Burning);
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
	Effects.Remove(EEffects::Burning);
	GetWorld()->GetTimerManager().ClearTimer(BurningTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(BurningDamageTimerHandle);
}

// Called when enemy armour is broken
void AEnemy::ApplyArmourBreak(int32 ArmourDamage, float BreakDuration)
{
	int Index = -1;

	if (CheckForImmunity(EEffects::Armour_Break, Index)) { return; }

	// Ñheck if enemy already has effect applied
	if (CheckForEffect(EEffects::Armour_Break, Index))
	{
		// Renew effect
		GetWorld()->GetTimerManager().ClearTimer(ArmourBreakTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ArmourBreakTimerHandle,
			this, &AEnemy::RemoveArmourBreak, BreakDuration, false);
	}
	else
	{
		Effects.Add(EEffects::Armour_Break);
		Armour -= ArmourDamage;
		GetWorld()->GetTimerManager().SetTimer(ArmourBreakTimerHandle,
			this, &AEnemy::RemoveArmourBreak, 
			BreakDuration, false);
	}

	
}

// Called when enemy takes damage over time from burning
void AEnemy::TakeDamageFromBurning(int32 BurningDamage)
{
	ReceiveDamage(BurningDamage);

	int32 Index;
	if (!CheckForEffect(EEffects::Burning, Index))
	{
		OnBurningDamageTimerFinished();
	}
}

// Apply slow on Enemy
void AEnemy::ApplySlow(float SlowStrength, float SlowDuration)
{
	int Index = -1;

	if (CheckForImmunity(EEffects::Slow, Index))
	{
		return;
	}

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

// Regenerates health
void AEnemy::Regenerate(int32 HealthToAdd)
{
	Health += HealthToAdd;
	Health = FMath::Clamp(Health, 0, MaxHealth);
}

float AEnemy::GetHealthPercentage() const
{
	return static_cast<float>(Health)/ static_cast<float>(MaxHealth);
}

FVector AEnemy::GetMeshLocation() const
{
	return Mesh->GetComponentLocation();
}

int32 AEnemy::GetDamage() const
{
	return DamageToBase;
}

// Removes slow from enemy
void AEnemy::RemoveSlow()
{
	Effects.RemoveSingle(EEffects::Slow);
	FloatingPawnComponent->MaxSpeed = InitialSpeed;
}

// Removes armour break
void AEnemy::RemoveArmourBreak()
{
	Effects.RemoveSingle(EEffects::Armour_Break);
	Armour = InitialArmour;
}

// Plays when enemy die
void AEnemy::Die()
{
	// Remove timers
	OnBurningDamageTimerFinished();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	// Destroy pawn
	RewardPlayer();
	Destroy();
}

// Reward player with a bounty
void AEnemy::RewardPlayer()
{
	TArray<AActor*> CameraPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraPlayer::StaticClass(), CameraPlayers);

	if ( auto Player = Cast<ACameraPlayer>(CameraPlayers[0]))
	{
		Player->AddBounty(Bounty);
	}
}

// Checks if enemy has that effect
// Return -1 in index if false
bool AEnemy::CheckForEffect(EEffects Effect, int32& Index)
{
	return Effects.Find(Effect, Index);
}

// Sets all enemy variables, that cannot be set on construction
void AEnemy::InitializeEnemy()
{
	Health = MaxHealth;
	Armour = InitialArmour;
	InitialSpeed = FloatingPawnComponent->MaxSpeed;
	RandomizeEnemyBounty();
}

bool AEnemy::CheckForImmunity(EEffects Effect, int32& Index)
{
	return Immunities.Find(Effect, Index);
}

// Called on BeginPlay to change bounty 
void AEnemy::RandomizeEnemyBounty()
{
	Bounty = FMath::RandRange(BountyBase - BountyDispersion, BountyBase + BountyDispersion);
}

// Called on BeginPlay, and when enemy is closed to target
void AEnemy::WalkToNextTarget()
{
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
			AIController->MoveToLocation(TargetLocation, 0.f, false, true, true, false);
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
		auto VDistance = Mesh->GetComponentLocation() - TargetLocation;
		float Distance = VDistance.Size();

		// Check if distance to target is less or equal then threshold
		if (Distance <= DistanceThreshold)
		{
			// Order enemy to walk to next target
			GetWorld()->GetTimerManager().ClearTimer(TimerProximityTimerHandle);
			WalkToNextTarget();
		}
	}
}