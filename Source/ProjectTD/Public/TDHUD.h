// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TDHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTTD_API UTDHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void DisplayBaseHealth(float Percentage);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
};
