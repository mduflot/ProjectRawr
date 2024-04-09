// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface_Hit.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

UCLASS()
class PROJECTRAWR_API AShield : public AActor, public IInterface_Hit
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShieldMesh;

	float LifeTime = 5.f;
	FTimerHandle AutoDestroyTimer;
	
	AShield();
	
	void Initialize();
	void AutoDestroy();
	void HitReaction_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
