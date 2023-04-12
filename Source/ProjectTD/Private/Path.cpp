// Fill out your copyright notice in the Description page of Project Settings.


#include "Path.h"
#include "Components/SplineComponent.h"

// Sets default values
APath::APath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize actor components
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Path = CreateDefaultSubobject<USplineComponent>("Path");
	Path->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void APath::BeginPlay()
{
	Super::BeginPlay();
	
}


// Gets location from spline index
void APath::GetLocationFromIndex(int32 Index, FVector& VectorRef)
{
	VectorRef = Path->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}
