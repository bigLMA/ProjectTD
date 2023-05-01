// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPanel.h"
#include "Turret.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "TurretUpgradeInfo.h"
#include "Components/TextBlock.h"

void UTurretPanel::DispalyPanel(ATurret* InTurret)
{
	if (!IsValid(InTurret)) { return; }

	Turret = InTurret;

	// Set turret info into widgets
	if (InTurret->GetThumbnail())
	{
		TurretImage->SetBrushFromTexture(InTurret->GetThumbnail());
	}
	TurretName->SetText(InTurret->GetTurretName());
	TurretDescription->SetText(InTurret->GetTurretDescription());

	// Remove all previous upgrade info wigdets
	UpgradeBox->ClearChildren();

	if (!TurretUpgradeClass) { return; }

	// Construct new info widgets
	for (int32 j = 0; j < Turret->GetUpgradesLength(); ++j)
	{
		// Create new widget
		UTurretUpgradeInfo* NewWidget = CreateWidget<UTurretUpgradeInfo>(
			GetWorld()->GetFirstPlayerController(),	TurretUpgradeClass);

		// Add widget to vertical box
		NewWidget->DisplayWidget(InTurret, j);
		UpgradeBox->AddChildToVerticalBox(NewWidget);
		NewWidget->SetPadding(FMargin(10.f));
	}
}

void UTurretPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Check if turret is valid
	if (!IsValid(Turret))
	{
		// Hide widget if not
		UpgradeBox->ClearChildren();
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
