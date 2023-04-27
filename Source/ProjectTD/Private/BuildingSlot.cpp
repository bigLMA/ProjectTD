// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSlot.h"
#include "Turret.h"
#include "TurretPrevievActor.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "CameraPlayer.h"


void UBuildingSlot::NativePreConstruct()
{
	// Set Widget info
	if (TurretClass)
	{
		//if (auto DefaultTurret = Cast<ATurret>(ATurret::StaticClass()->GetDefaultObject()))
		if(auto DefaultTurret = TurretClass.GetDefaultObject())
		{
			if (auto Thumbnail = DefaultTurret->GetThumbnail())
			{
				Image->SetBrushFromTexture(Thumbnail);
			}
			CostLabel->SetText(FText::FromString(FString::FromInt(DefaultTurret->GetCost())));
			Cost = DefaultTurret->GetCost();
		}
	}
}

// Called when mouse is down on widget
FReply UBuildingSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Check if left mouse butoon is pressed
	if (!InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton"))) { return FReply::Handled(); }

	if (auto Player = Cast<ACameraPlayer>(
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		if (Player->GetMoney() > Cost)
		{
			// Create turret preview actor
			FActorSpawnParameters Params;
			auto PreviewActor = GetWorld()->SpawnActor<ATurretPrevievActor>(PreviewClass, Params);

			if (PreviewActor)
			{
				PreviewActor->SetTurretToBuild(TurretClass);
				Player->SetPreviewActor(PreviewActor);
				Player->ToggleConstructionVisibility(true);
			}
		}
	}
	
	return FReply::Handled();
}
