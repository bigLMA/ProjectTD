// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretPlacingActor.generated.h"

UCLASS()
class PROJECTTD_API ATurretPlacingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretPlacingActor();

	// Toggle visibility of mesh
	UFUNCTION(BlueprintCallable, Category = "Toggle Visibility")
	void ToggleActorVisibility(bool bNewVisibility);

	UFUNCTION(BlueprintCallable, Category = "Errect turret")
	void ErrectTurret(TSubclassOf<class ATurret> TurretClass);

	UFUNCTION(BlueprintCallable, Category = "Placed status")
	bool GetIsPlaced() const { return bIsPlaced; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Actor components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* Mesh;

private:
	// Is turret already placed?
	bool bIsPlaced;

};
