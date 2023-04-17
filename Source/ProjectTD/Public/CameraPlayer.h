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

private:
	// Input functions
	void Move(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);
	void Select(const FInputActionValue& Value);

	// Income
	void AddIncome();
};
