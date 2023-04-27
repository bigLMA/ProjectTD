// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPlacingActor.h"
#include "Turret.h"
#include "CameraPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATurretPlacingActor::ATurretPlacingActor()
	:bIsPlaced(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize components
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ATurretPlacingActor::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->SetVisibility(false);
}

// Toggle visibility of mesh
void ATurretPlacingActor::ToggleActorVisibility(bool bNewVisibility)
{
	if (bIsPlaced)
	{
		Mesh->SetVisibility(false);
		return;
	}

	Mesh->SetVisibility(bNewVisibility);
}

void ATurretPlacingActor::ErrectTurret(TSubclassOf<class ATurret> TurretClass)
{
	if (bIsPlaced) { return; }

	// Spawn turret
	FActorSpawnParameters Params;
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	auto Turret = GetWorld()->SpawnActor<ATurret>(TurretClass, Location, Rotation, Params);	
	bIsPlaced = true;

	// Remove player money
	if (auto Player = Cast<ACameraPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		Player->RemovePlayerMoney(Turret->GetCost());
		Player->ToggleConstructionVisibility(false);
	}
}

