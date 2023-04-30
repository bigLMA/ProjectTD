// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurretProjectile.h"
#include "CameraPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurret::ATurret()
	:RotationSpeed(9.5f), 
	Cost(10),
	ReloadTime(0.6f),
	Damage(15),
	DamageToEnemies(1),
	DamageToShields(1), 
	SplashDamageModifier(0.3f),
	FireRange(500.f),
	MultitargetDamageModifier(0.1f),
	MultitargetRadius(400.f)
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

	// Check upgrade possibility
	if (!CheckForUpgrade(UpgradeType, Index, 1)||
		!IsValid(Upgrades[Index].Value)|| 
		!(Upgrades[Index].CurrentLevel < Upgrades[Index].MaxLevel))
	{ return; }

	// Apply upgrades
	switch (UpgradeType)
	{
	case EUpgradeType::Damage:
		Damage = static_cast<int32>(Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel));
		break;
	case EUpgradeType::DamageToEnemies:
		DamageToEnemies = Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		break;
	case EUpgradeType::DamageToShields:
		DamageToShields = Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		break;
	case EUpgradeType::ReloadTime:
		ReloadTime= Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		break;
	case EUpgradeType::Splash:
		SplashDamageModifier = Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		break;
	case EUpgradeType::Multitarget:
		MultitargetDamageModifier = Upgrades[Index].Value->GetFloatValue(++Upgrades[Index].CurrentLevel);
		break;
	default:
		break;
	}

	if (auto Player = Cast<ACameraPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Player->RemovePlayerMoney(Upgrades[Index].UpgradeCost->GetFloatValue(Upgrades[Index].CurrentLevel));
	}
}

int32 ATurret::GetCost() const
{
	return Cost;
}

UTexture2D* ATurret::GetThumbnail() const
{
	return Thumbnail;
}

int32 ATurret::GetUpgradesLength() const
{
	return Upgrades.Num();
}

const FUpgrades& ATurret::GetUpgradeAtIndex(int32 Index) const
{
	return Upgrades[Index];
}

const FText& ATurret::GetTurretName() const
{
	return TurretName;
}

const FText& ATurret::GetTurretDescription() const
{
	return TurretDescription;
}

// Locks on target
void ATurret::LockOn(const AActor* Target, float Time)
{
	if (!IsValid(Target) || GetDistanceTo(Target)>FireRange)
	{
		OnTargetLost.Broadcast(GetActorLocation());
		return;
	}

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

		int32 Index;

		if (CheckForUpgrade(EUpgradeType::Multitarget, Index))
		{
			OnShoot.Broadcast(MultitargetRadius);
		}
	}
}

// Check for at least one level of upgrade if UpgradeIndex is not set
// Returns level of upgrade if has one and index in array
bool ATurret::CheckForUpgrade(EUpgradeType UpgradeType, int32& Index, int32 UpgradeIndex)
{
	for (int32 j = 0; j < Upgrades.Num(); ++j)
	{
		if (Upgrades[j].UpgradeType == UpgradeType && Upgrades[j].CurrentLevel+UpgradeIndex > 0)
		{
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
	int32 Index;
	int32 SplashDamage = (CheckForUpgrade(EUpgradeType::Splash, Index)) ? static_cast<int32>(Damage * SplashDamageModifier): -1;

	Projectile->ActivateProjectile(Target, Damage, DamageToEnemies, DamageToShields, SplashDamage);
}