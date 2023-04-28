// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetLostDelegate, const FVector& , Location)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultitargetShootingDelegate, float, Radius)

UENUM()
enum class EUpgradeType : uint8
{
	Damage, DamageToEnemies, DamageToShields, Slow, BurningDamage, ReloadTime, Splash, Multitarget, ArmourPenetrarion
};

USTRUCT(BlueprintType)
struct FUpgrades
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade icon")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Description")
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade type")
	EUpgradeType UpgradeType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrade cost")
	UCurveFloat* UpgradeCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Current level")
	int32 CurrentLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Max level")
	int32 MaxLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Value per level")
	UCurveFloat* Value;
};

UCLASS()
class PROJECTTD_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Aim at special location
	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void AimAt(const AActor* Target);

	// Fires at target
	void Fire(const AActor* Target);

	// Upgrade turret
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	virtual void Upgrade(EUpgradeType UpgradeType);

	// Getters to pass turret info
	int32 GetCost() const;
	UTexture2D* GetThumbnail() const;
	int32 GetUpgradesLength() const;
	const FUpgrades& GetUpgradeAtIndex(int32 Index) const;
	const FText& GetTurretName() const;
	const FText& GetTurretDescription() const;

	// Delegate, called when target is lost
	FTargetLostDelegate OnTargetLost;

	// Delegate to shoot additional targets
	FMultitargetShootingDelegate OnShoot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Name of turret
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
	FText TurretName;

	// Turret descriptiopn, a brief text of what turret is doing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Description")
	FText TurretDescription;

	// Turret thumbnail
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Thumbnail")
	UTexture2D* Thumbnail;

	// Cost of turret to build
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cost", meta = (ClampMin = 1, UIMin = 1))
	int32 Cost;

	// Damage of turret
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = 1, UIMin = 1))
	int32 Damage;

	// Damage to multiply when damaging enemies
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 3.f, UIMax = 3.f))
	float DamageToEnemies;

	// Damage to multiply when damaging shields
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 3.f, UIMax = 3.f))
	float DamageToShields;

	// Speed of firing new projectiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire rate", meta = (ClampMin = 0.1, UIMin = 0.1))
	float ReloadTime;

	// How much the damage from splash would be modified
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Splash damage modifier", meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax= 1.2, UIMax = 1.2))
	float SplashDamageModifier;

	// How much the damage from multitarget would be modified
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multitarget damage modifier", meta = (ClampMin = 0.1, UIMin = 0.1, ClampMax =0.8, UIMax = 0.8))
	float MultitargetDamageModifier;

	// Possible upgrades of turret
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Upgrades")
	TArray<FUpgrades> Upgrades;

	//Class of projectile fired
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile class")
	TSubclassOf<class ATurretProjectile> ProjectileClass;

	// Turret rotation per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret rotation")
	float RotationSpeed;

	// Turret fire range
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fire range")
	float FireRange;

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Turret")
	UStaticMeshComponent* Tower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	UStaticMeshComponent* Base;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shoot Colision")
	class USphereComponent* ShootColision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shoot Decal")
	UDecalComponent* ShootDecal;

	// Sets projectile variables, activates movement
	UFUNCTION(BlueprintCallable, Category = "Projectile setting")
	virtual void PrepareProjectile(ATurretProjectile* Projectile, const AActor* Target);

	// Check for at least one level of upgrade if UpgradeIndex is not set
	// Returns level of upgrade if has one and index in array
	bool CheckForUpgrade(EUpgradeType UpgradeType, int32& Index, int32 UpgradeIndex = 0);

private:
	// Aiming timer
	FTimerHandle AimingTimerHandle;

	// Locks on target
	void LockOn(const AActor* Target, float Time);

	// MultitargetRadius
	float MultitargetRadius;

	double LastFireTime;
};
