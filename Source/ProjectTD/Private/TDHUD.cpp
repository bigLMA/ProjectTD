// Fill out your copyright notice in the Description page of Project Settings.


#include "TDHUD.h"
#include "Components/ProgressBar.h"

void UTDHUD::DisplayBaseHealth(float Percentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percentage);
	}
}
