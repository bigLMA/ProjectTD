// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Path.generated.h"

UCLASS()
class PROJECTTD_API APath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APath();

	// Gets location from spline index
	UFUNCTION(BlueprintCallable, Category = "Spline Path")
	void GetLocationFromIndex(int32 Index, FVector& VectorRef);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Used to build path for enemies
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Path")
	class USplineComponent* Path;

	// Root of actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* Root;
};
