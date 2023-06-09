// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "TDHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TurretPrevievActor.h"
#include "TurretPlacingActor.h"
#include "Turret.h"

// Sets default values
ACameraPlayer::ACameraPlayer()
	:ZoomMin(900.f), ZoomMax(4500.f), ZoomSpeed(-95.f), Money(10), Income(2)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize components
	Root = CreateDefaultSubobject< USceneComponent>("Root");
	SetRootComponent(Root);

	SpringArm = CreateDefaultSubobject< USpringArmComponent>("Spring arm");
	SpringArm->SetupAttachment(Root);
	SpringArm->TargetArmLength = 3500.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	PawnMovement = CreateDefaultSubobject< UFloatingPawnMovement>("Pawn movement");
}

// Called when the game starts or when spawned
void ACameraPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast< APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	// Launch income
	FTimerHandle IncomeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(IncomeTimerHandle,
		this, &ACameraPlayer::AddIncome, 1.f, true);

	// Create widget
	if (HUDClass)
	{
		PlayerHUD = CreateWidget<UTDHUD>(UGameplayStatics::GetPlayerController
		(GetWorld(), 0), HUDClass);
		PlayerHUD->AddToViewport();
		PlayerHUD->DisplayBaseHealth(1.f);
	}
}

// Called to bind functionality to input
void ACameraPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Input
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInput->BindAction(MoveCamera, ETriggerEvent::Triggered, this, &ACameraPlayer::Move);
		EnhancedInput->BindAction(ZoomCamera, ETriggerEvent::Triggered, this, &ACameraPlayer::Zoom);
		EnhancedInput->BindAction(RotateCamera, ETriggerEvent::Triggered, this, &ACameraPlayer::Rotate);

		// Clicking
		EnhancedInput->BindAction(SelectUnit, ETriggerEvent::Triggered, this, &ACameraPlayer::Select);
		EnhancedInput->BindAction(CancelAction, ETriggerEvent::Triggered, this, &ACameraPlayer::Cancel);
	}
}

// Moves pawn
void ACameraPlayer::Move(const FInputActionValue& Value)
{
	auto MoveValue = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MoveValue.Y);
	AddMovementInput(GetActorRightVector(), MoveValue.X);
}

// Camera zoom
void ACameraPlayer::Zoom(const FInputActionValue& Value)
{
	auto ZoomValue = Value.Get<float>();
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + ZoomValue * ZoomSpeed, ZoomMin, ZoomMax);
}

// Rotates pawn
void ACameraPlayer::Rotate(const FInputActionValue& Value)
{
	auto RotateValue = Value.Get<float>();
	SetActorRotation(GetActorRotation() + FRotator(0.f, RotateValue, 0.f));
}

// Select interactable actor
void ACameraPlayer::Select(const FInputActionValue& Value)
{
	auto Selection = Value.Get<bool>();
	if (Selection)
	{
		if (PreviewActor)
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> Object;
			Object.Add(EObjectTypeQuery::ObjectTypeQuery8);
			FHitResult Hit;

			if (GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects(Object, false, Hit))
			{
				PreviewActor->ConstructTurret();
				PreviewActor = nullptr;
				return;
			}
		}
	}

	FHitResult Hit;

	if (Selection && GetWorld()->GetFirstPlayerController()->
		GetHitResultUnderCursorByChannel(
			TraceTypeQuery1, false, Hit))
	{
		if (auto Turret = Cast<ATurret>(Hit.GetActor()))
		{
			PlayerHUD->ToggleTurretInfo(true, Turret);
		}
		else
		{
			PlayerHUD->ToggleTurretInfo(false, nullptr);
		}
	}
}

void ACameraPlayer::Cancel(const FInputActionValue& Value)
{
	auto Canceling = Value.Get<bool>();
	if (Canceling)
	{
		if (PreviewActor)
		{
			PreviewActor->Destroy();
			PreviewActor = nullptr;
			ToggleConstructionVisibility(false);
			return;
		}
		else
		{
			PlayerHUD->ToggleTurretInfo(false, nullptr);
		}
	}
}

// Income
void ACameraPlayer::AddIncome()
{
	Money += Income;

	if (PlayerHUD)
	{
		PlayerHUD->DisplayMoney(Money);
	}
}

// Adds bounty to money on killing enemy
void ACameraPlayer::AddBounty(int32 Bounty)
{
	Money += Bounty;

	if (PlayerHUD)
	{
		PlayerHUD->DisplayMoney(Money);
	}
}

void ACameraPlayer::RemovePlayerMoney(int32 MoneyToRemove)
{
	Money -= MoneyToRemove; 

	if (PlayerHUD)
	{
		PlayerHUD->DisplayMoney(Money);
	}
}

void ACameraPlayer::UpdateBaseHealth(float Percentage)
{
	PlayerHUD->DisplayBaseHealth(Percentage);
}

void ACameraPlayer::ToggleConstructionVisibility(bool bIsVisble)
{
	TArray<AActor*> TurretPlacingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurretPlacingActor::StaticClass(), TurretPlacingActors);

	for (auto Actor : TurretPlacingActors)
	{
		if (auto Placing = Cast<ATurretPlacingActor>(Actor))
		{
			Placing->ToggleActorVisibility(bIsVisble);
		}
	}
}

int32 ACameraPlayer::GetMoney() const
{
	return Money;
}

void ACameraPlayer::SetPreviewActor(ATurretPrevievActor* Actor)
{
	PreviewActor = Actor;
}
