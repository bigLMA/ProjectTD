// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATurret::ATurret()
	:RotationSpeed(4.5f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Components
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Base = CreateDefaultSubobject<UStaticMeshComponent>("Base");
	Base->SetupAttachment(Root);

	ShootDecal = CreateDefaultSubobject<UDecalComponent>("Shoot Decal");
	ShootDecal->SetupAttachment(Root);
	ShootDecal->SetAutoActivate(false);
	//ShootDecal->SetActive(false);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
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
	float Time = 0.1f;

	if (Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target is set to %s"), *Target->GetName());
		//TODO
		GetWorld()->GetTimerManager().SetTimer(AimingTimerHandle, [this, Target, Time]()
		{
			LockOn(Target, Time);
		}, Time, true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy named left"));
		GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	}
}



void ATurret::LockOn(const AActor* Target, float Time)
{
	UE_LOG(LogTemp, Warning, TEXT("Locking onto target %s"), *Target->GetName());
	auto Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	auto NewRot = FMath::RInterpTo(GetActorRotation(), Rot, Time, RotationSpeed);
	FRotator LockRotation(0, NewRot.Yaw, 0);

	SetActorRotation(LockRotation);
}