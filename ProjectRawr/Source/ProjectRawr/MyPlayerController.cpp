// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CreatePlayerHUD();

	FString TextToPrint = UKismetSystemLibrary::GetDisplayName(this);
	TextToPrint.Append("::BeginPlay().");
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow, TextToPrint);
}

void AMyPlayerController::CreatePlayerHUD()
{
	if (!PlayerHUD)
	{
		if (MyWidgetClass)
		{
			PlayerHUD = CreateWidget<UMyPlayerHUD>(this, MyWidgetClass);
			if (PlayerHUD)
				PlayerHUD->AddToViewport(0);
		}
	}
}
