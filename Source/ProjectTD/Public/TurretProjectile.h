// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurretProjectile.generated.h"

UCLASS()
class PROJECTTD_API ATurretProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurretProjectile();

	// Called when turret shoots with projectile
	void ActivateProjectile(const AActor* Target, int32 DamageToSet, float EnemyDamage, float ShieldDamage, int32 Splash = -1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile mesh")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile collision")
	class USphereComponent* SplashCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile mesh")
	float ProjectileSpeed;

	// Called when hits
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Damage multipliers for different target types
	float DamageToEnemies;
	float DamageToShields;

	// Damage to deliver
	int32 Damage;

private:
	// Splash damage
	int32 SplashDamage;

	// Splash radius
	float SplashRadius;
};
