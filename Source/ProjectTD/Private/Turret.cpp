// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretProjectile.h"

// Sets default values
ATurret::ATurret()
	:RotationSpeed(9.5f), 
	Cost(10),
	ReloadTime(0.6f),
	Damage(15),
	DamageToEnemies(1),
	DamageToShields(1), 
	SplashDamageModifier(0.2f),
	MultitargetDamageModifier(0.1f)
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
	ShootColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ShootDecal = CreateDefaultSubobject<UDecalComponent>("Shoot Decal");
	ShootDecal->SetupAttachment(Root);
	ShootDecal->SetVisibility(false);
	//ShootDecal->SetAutoActivate(false);
	//ShootDecal->SetActive(false);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

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

// Upgrade turret
void ATurret::Upgrade(EUpgradeType UpgradeType)
{
	int32 Index;
	int32 Level;

	if (!CheckForUpgrade(UpgradeType, Level, Index, 1)) { UE_LOG(LogTemp, Warning, TEXT("NO DAMAGE"));
	return; }

	switch (UpgradeType)
	{
	case EUpgradeType::Damage:
		Damage += static_cast<int32>(Upgrades[Index].ValuePerLevel);
		++Upgrades[Index].CurrentLevel;
		break;
	case EUpgradeType::DamageToEnemies:
		break;
	case EUpgradeType::DamageToShields:
		break;
	case EUpgradeType::ReloadTime:
		break;
	case EUpgradeType::Splash:
		break;
	case EUpgradeType::Multitarget:
		break;
	default:
		break;
	}
}

// Locks on target
void ATurret::LockOn(const AActor* Target, float Time)
{
	// Calculate new rotation for tower
	auto Rot = UKismetMathLibrary::FindLookAtRotation(Tower->GetComponentLocation(), Target->GetActorLocation());
	auto NewRot = FMath::RInterpTo(Tower->GetComponentRotation(), Rot, Time, RotationSpeed);
	FRotator LockRotation(0, NewRot.Yaw, 0);

	// Rotate tower
	Tower->SetWorldRotation(LockRotation);

	// Calculate if tower is rotated enogh to fire
	float RelativeYaw = Tower->GetComponentRotation().Yaw;
	float NewRotYaw = Rot.Yaw;
	float Diff = RelativeYaw - NewRotYaw;
	float MinDiff = 25.f;

	// Check if turret can fire 
	if(FMath::Abs(Diff) < MinDiff && (FPlatformTime::Seconds() - LastFireTime) >= ReloadTime)
	{
		Fire(Target);
	}
}

// Check for at least one level of upgrade.
// Returns level of upgrade if has one and index in array
// "int32 UpgradeIndex" is for checking if class has ability to upgrade, not current upgrades
bool ATurret::CheckForUpgrade(EUpgradeType UpgradeType, int32& UpgradeLevel, int32& Index, int32 UpgradeIndex)
{
	for (int32 j = 0; j < Upgrades.Num(); ++j)
	{
		if (Upgrades[j].UpgradeType == UpgradeType && Upgrades[j].CurrentLevel+UpgradeIndex > 0)
		{
			UpgradeLevel = Upgrades[j].CurrentLevel;
			Index = j;
			return true;
		}
	}

	return false;
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
		PrepareProjectile(Projectile, Target);
	}
	else
	{
		const FVector& Location = GetActorLocation();
		OnTargetLost.Broadcast(Location);
	}
}

// Sets projectile variables, activates movement
void ATurret::PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target)
{
	Projectile->ActivateProjectile(Target, Damage, DamageToEnemies, DamageToShields);
}