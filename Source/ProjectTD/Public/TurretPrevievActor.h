// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretPrevievActor.generated.h"

UCLASS()
class PROJECTTD_API ATurretPrevievActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretPrevievActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPrevievLocation();

	UFUNCTION(BlueprintCallable, Category = "Turret to build")
	void SetTurretToBuild(TSubclassOf<class ATurret> TurretClass);

	// Checks if can construct turret
	// Spawns turret if can
	UFUNCTION(BlueprintCallable, Category = "Turret construction")
	void ConstructTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Allows change material of actor accordingly to construct allowance
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Actor material")
	void SetActorMaterial( bool bCanConstruct);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret class")
	TSubclassOf<class ATurret> TurretToBuild;

	// Actor components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Root")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	UStaticMeshComponent* Base;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tower")
	UStaticMeshComponent* Tower;

private:
	TArray<TEnumAsByte<EObjectTypeQuery>> Object;

	TArray<TEnumAsByte<EObjectTypeQuery>> Ground;
};
