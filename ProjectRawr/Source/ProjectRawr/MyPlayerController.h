// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAWR_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyPlayerHUD> MyWidgetClass;

	UMyPlayerHUD* PlayerHUD = nullptr;

	void CreatePlayerHUD();

protected:
	virtual void BeginPlay() override;
};
