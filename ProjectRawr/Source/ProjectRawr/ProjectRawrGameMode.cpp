// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRawrGameMode.h"
#include "ProjectRawrCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectRawrGameMode::AProjectRawrGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
