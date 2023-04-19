// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretProjectile.h"

// Sets default values
ATurret::ATurret()
	:RotationSpeed(9.5f), Cost(10), ReloadTime(0.6f), Damage(15)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Components
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Base = CreateDefaultSubobject<UStaticMeshComponent>("Base");
	Base->SetupAttachment(Root);

	Tower = CreateDefaultSubobject<UStaticMeshComponent>("Tower");
	Tower->SetupAttachment(Base);

	ShootColision = CreateDefaultSubobject<USphereComponent>("Shoot collision");
	ShootColision->SetupAttachment(Tower);

	ShootDecal = CreateDefaultSubobject<UDecalComponent>("Shoot Decal");
	ShootDecal->SetupAttachment(Root);
	ShootDecal->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	ShootDecal->SetActive(false);
	LastFireTime = FPlatformTime::Seconds();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Aim at special location
void ATurret::AimAt(const AActor* Target)
{
	float Time = 0.015f;

	if (IsValid(Target))
	{
		GetWorld()->GetTimerManager().SetTimer(AimingTimerHandle, [this, Target, Time]()
		{
			LockOn(Target, Time);
		}, Time, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	}
}

// Locks on target
void ATurret::LockOn(const AActor* Target, float Time)
{
	// Calculate new rotation for tower
	auto Rot = UKismetMathLibrary::FindLookAtRotation(Tower->GetComponentLocation(), Target->GetActorLocation());
	auto NewRot = FMath::RInterpTo(Tower->GetRelativeRotation(), Rot, Time, RotationSpeed);
	FRotator LockRotation(0, NewRot.Yaw, 0);

	// Rotate tower
	Tower->SetRelativeRotation(LockRotation);

	// Calculate if tower is rotated enogh to fire
	float RelativeYaw = Tower->GetRelativeRotation().Yaw;
	float NewRotYaw = Rot.Yaw;
	float Diff = RelativeYaw - NewRotYaw;
	float MinDiff = 7.5f;

	// Check if turret can fire 
	if(FMath::Abs(Diff) < MinDiff && (FPlatformTime::Seconds() - LastFireTime) >= ReloadTime)
	{
		Fire(Target);
	}
}

void ATurret::Fire(const AActor* Target)
{
	if (IsValid(Target))
	{
		LastFireTime = FPlatformTime::Seconds();
		FActorSpawnParameters Params;
		auto Projectile = GetWorld()->SpawnActor<ATurretProjectile>(ProjectileClass,
			ShootColision->GetComponentLocation(), Tower->GetRelativeRotation(),
			Params);
		Projectile->ActivateProjectile(Target, Damage);
	}
	else
	{
		const FVector& Location = GetActorLocation();
		OnTargetLost.Broadcast(Location);
	}
}
