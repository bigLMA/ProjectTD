// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "CameraPlayer.generated.h"

UCLASS()
class PROJECTTD_API ACameraPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPlayer();

	// Adds bounty to money on killing enemy
	void AddBounty(int32 Bounty);
	
	// Removes player money
	UFUNCTION(BlueprintCallable, Category = "Money")
	void RemovePlayerMoney(int32 MoneyToRemove);

	// Widget progress bar function
	void UpdateBaseHealth(float Percentage);

	// Set all placing actors to toggle visibility
	void ToggleConstructionVisibility(bool bIsVisble);

	int32 GetMoney() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Actor components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	class UFloatingPawnMovement* PawnMovement;

	// Enhanced input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Mapping Context")
	class UInputMappingContext* MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Move")
	class UInputAction* MoveCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Zoom")
	UInputAction* ZoomCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Rotate")
	UInputAction* RotateCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Select")
	UInputAction* SelectUnit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Cancel")
	UInputAction* CancelAction;

	// Minimal spring arm length
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Zoom")
	float ZoomMin;

	// Maximum spring arm length
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Zoom")
	float ZoomMax;

	// Speed of spring arm
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Zoom")
	float ZoomSpeed;

	// Player Money
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Money", meta = (ClampMin = 0, UIMin = 0))
	int32 Money;

	// Player income (per second)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Money", meta = (ClampMin = 0, UIMin = 0))
	int32 Income;

	// HUD class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UTDHUD> HUDClass;

	// HUD instance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTDHUD* PlayerHUD;

private:
	// Input functions
	void Move(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void Select(const FInputActionValue& Value);
	void Cancel(const FInputActionValue& Value);

	// Income
	void AddIncome();

	// Preview actor
	class ATurretPrevievActor* PreviewActor;

public:
	void SetPreviewActor(ATurretPrevievActor* Actor);
};
