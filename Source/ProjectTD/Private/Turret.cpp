// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATurret::ATurret()
	:RotationSpeed(6.5f), Cost(10), ReloadTime(1.2f), Damage(15)
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

	ShootDecal = CreateDefaultSubobject<UDecalComponent>("Shoot Decal");
	ShootDecal->SetupAttachment(Root);
	ShootDecal->SetAutoActivate(false);
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

// Fire with at target with projectile
void ATurret::Fire()
{
	//TODO
}

// Aim at special location
void ATurret::AimAt(const AActor* Target)
{
	float Time = 0.015f;

	if (Target)
	{
		//TODO
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
	LastFireTime = FPlatformTime::Seconds();
	// TODO Spawn projectile
	UE_LOG(LogTemp, Warning, TEXT("Foier"));
}
