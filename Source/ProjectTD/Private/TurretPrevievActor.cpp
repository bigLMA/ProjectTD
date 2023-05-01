// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPrevievActor.h"
#include "TurretPlacingActor.h"
#include "Turret.h"

// Sets default values
ATurretPrevievActor::ATurretPrevievActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize Components
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Base = CreateDefaultSubobject<UStaticMeshComponent>("Base");
	Base->SetupAttachment(Root);
	Base->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Tower = CreateDefaultSubobject<UStaticMeshComponent>("Tower");
	Tower->SetupAttachment(Base);
	Tower->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATurretPrevievActor::BeginPlay()
{
	Super::BeginPlay();
	// set Object for tracing turret placing actor
	Object.Add(EObjectTypeQuery::ObjectTypeQuery8);

	// set Ground for tracing ground objects
	Ground.Add(EObjectTypeQuery::ObjectTypeQuery9);
}

// Called every frame
void ATurretPrevievActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetPrevievLocation();

	//ConstructTurret();
}

void ATurretPrevievActor::SetPrevievLocation()
{
	FHitResult Hit;

	if (GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(Object, false, Hit))
	{
		if (auto PlacingActor = Cast<ATurretPlacingActor>(Hit.GetActor()))
		{
			if (!PlacingActor->GetIsPlaced())
			{
				// Inform player of building allowance and location
				SetActorLocation(PlacingActor->GetActorLocation());
				SetActorMaterial(true);
			}
		}
	}
	else if (GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(Ground, false, Hit))
	{
		// Just show actor and unavailable building status 
		SetActorLocation(Hit.Location);
		SetActorMaterial(false);
	}
}

void ATurretPrevievActor::SetTurretToBuild(TSubclassOf<class ATurret> TurretClass)
{
	TurretToBuild = TurretClass;
}

void ATurretPrevievActor::ConstructTurret()
{
	if (!IsValid(TurretToBuild)) { return; }

	FHitResult Hit;

	if (GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(Object, false, Hit))
	{
		if (auto PlacingActor = Cast<ATurretPlacingActor>(Hit.GetActor()))
		{
			if (!PlacingActor->GetIsPlaced())
			{
				PlacingActor->ErrectTurret(TurretToBuild);
				Destroy();
			}
		}
	}
}

