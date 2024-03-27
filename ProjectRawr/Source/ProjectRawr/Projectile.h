// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class PROJECTRAWR_API AProjectile : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Sphere;
	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionEmitter;

	UParticleSystemComponent* ExplosionVFX;
	FVector ThrowVelocity;
	APawn* PawnSource;

	float LifeTime = 10.f;
	FTimerHandle AutoDestroyTimer;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);
	void Initialize(FVector NewVelocity, APawn* NewPawnSource);
	void AutoDestroy();

	// Sets default values for this actor's properties
	AProjectile();
};
