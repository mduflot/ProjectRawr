// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Interface_Health.h"
#include "Interface_Hit.h"
#include "Projectile.h"
#include "Shield.h"
#include "ProjectRawrCharacter.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class PROJECTRAWR_API AMyCharacter : public ACharacter, public IInterface_Hit, public IInterface_Health
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* MyInputMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ShootInputAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ShieldInputAction;

	UPROPERTY(EditAnywhere)
	UInputAction* SpawnInputAction;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* RegularMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* CooldownMaterial;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* ShieldCooldownMaterial;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> MyProjectileClass = AProjectile::StaticClass();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AShield> MyShieldClass = AShield::StaticClass();

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> MyPetClass = APawn::StaticClass();

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* FollowCamera;

	float ShootVelocity = 2500.f;
	FTimerHandle ShootCooldownTimer;
	FTimerHandle ShieldCooldownTimer;
	FTimerHandle SpawnCooldownTimer;
	FTimerHandle RetryTimer;
	float ShootCooldownDuration = 2.f;
	float ShieldCooldownDuration = 5.f;
	float SpawnCooldownDuration = 10.f;

	// Sets default values for this character's properties
	AMyCharacter();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void HitReaction_Implementation(FVector HitDirection, APawn* HitInstigator) override;
	virtual int GetHealth_Implementation() override;
	

	UFUNCTION(Client, Unreliable)
	void InitializeColor_Client();

	void InitializeInputMapping();
	FTransform GetShootStartTransform();
	FVector GetShootDirection(FVector StartLocation);
	void StartShootCooldown();
	void EndShootCooldown();
	void StartShieldCooldown();
	void EndShieldCooldown();
	void StartSpawnCooldown();
	void EndSpawnCooldown();

	void Look(const FInputActionValue& ActionValue);
	void TryShoot();
	void TryShield();
	void TrySpawn();

	UFUNCTION(Server, Unreliable)
	void Shoot_Server(FTransform StartTransform, FVector Velocity);

	UFUNCTION(Server, Unreliable)
	void Shield_Server(FTransform StartTransform, FVector Velocity);

	UFUNCTION(Server, Unreliable)
	void Spawn_Server(FTransform StartTransform, FVector Velocity);
};
