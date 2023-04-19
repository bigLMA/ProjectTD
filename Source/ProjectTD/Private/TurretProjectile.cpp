// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"

// Sets default values
ATurretProjectile::ATurretProjectile()
	:Damage(0), ProjectileSpeed(15.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize components
//	ProjectileCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	//ProjectileCollision->
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionObjectType(ECC_GameTraceChannel7);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Block);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile movement");
	ProjectileMovement->SetAutoActivate(false);
	ProjectileMovement->MaxSpeed = 500.f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 15000.f;
}

// Called when the game starts or when spawned
void ATurretProjectile::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &ATurretProjectile::OnHit);
	
}

//
void ATurretProjectile::ActivateProjectile(const AActor* Target, int32 DamageToSet)
{
	// Launch projectile
	ProjectileMovement->SetActive(true);
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileSpeed);

	// Initialize projectile self-guided
	if (auto HomingTarget = Target->FindComponentByClass<UStaticMeshComponent>())
	{
		ProjectileMovement->HomingTargetComponent = HomingTarget;
	}

	Damage = DamageToSet;
}

//
void ATurretProjectile::OnHit(AActor* HitActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	

	if (auto Enemy = Cast<AEnemy>(OtherActor))
	{
		Enemy->ReceiveDamage(Damage);
	}
	else
	{
		//TODO for shields
	}
	Destroy();
}