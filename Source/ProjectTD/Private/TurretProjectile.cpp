// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"
#include "ShieldComponent.h"

// Sets default values
ATurretProjectile::ATurretProjectile()
	:ProjectileSpeed(1500.f), Damage(0), DamageToEnemies(0.f), DamageToShields(0.f), SplashDamage(-1), SplashRadius(550.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize components
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionObjectType(ECC_GameTraceChannel7);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Block);

	SplashCollision = CreateDefaultSubobject<USphereComponent>("Splash Collision");
	SplashCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SplashCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SplashCollision->SetupAttachment(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movement");
	ProjectileMovement->SetAutoActivate(false);
	ProjectileMovement->MaxSpeed = 750.f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 330000.f;
}

// Called when the game starts or when spawned
void ATurretProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ATurretProjectile::OnHit);
	SplashCollision->SetSphereRadius(SplashRadius);
}

// Called when turret shoots with projectile
void ATurretProjectile::ActivateProjectile(const AActor* Target, int32 DamageToSet, float EnemyDamage, float ShieldDamage, int32 Splash)
{
	// Set damage modifiers
	DamageToEnemies = EnemyDamage;
	DamageToShields = ShieldDamage;

	// Launch projectile
	ProjectileMovement->SetActive(true);
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileSpeed);

	// Initialize projectile self-guided
	if (auto HomingTarget = Target->FindComponentByClass<UStaticMeshComponent>())
	{
		ProjectileMovement->HomingTargetComponent = HomingTarget;
	}

	Damage = DamageToSet;
	SplashDamage = Splash;
}

// Called when hits
void ATurretProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check for splash damage
	if (SplashDamage > 0&& IsValid(SplashCollision))
	{
		TArray<AActor*> Actors;

		SplashCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplashCollision->GetOverlappingActors(Actors);

		// Apply splash damage for all enemies exept Hit actor
		for (auto Actor : Actors)
		{
			if (Actor != OtherActor)
			{
				if (auto Enemy = Cast<AEnemy>(Actor))
				{
					Enemy->ReceiveDamage(SplashDamage);
				}
			}
		}
	}

	if (auto Shield = Cast<UShieldComponent>(OtherComponent))
	{
		// Calculate damage to shield
		float FDamageToShield = static_cast<float>(Damage) * DamageToShields;
		int32 DamageToShield = static_cast<int32>(FDamageToShield);

		// Damage shield
		Shield->RecieveDamage(DamageToShield);

		// Destroy projectile
		Destroy();
		return;
	}
	else if (auto Enemy = Cast<AEnemy>(OtherActor))
	{
		// Calculate damage to enemy
		float FDamageToEnemy = static_cast<float>(Damage) * DamageToEnemies;
		int32 DamageToEnemy = static_cast<int32>(FDamageToEnemy);

		// Damage Enemy
		Enemy->ReceiveDamage(DamageToEnemy);

		// Destroy projectile
		Destroy();
		return;
	}

	Destroy();
}