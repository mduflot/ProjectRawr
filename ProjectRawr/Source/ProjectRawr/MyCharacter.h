// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Interface_Hit.h"
#include "Projectile.h"
#include "ProjectRawrCharacter.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class PROJECTRAWR_API AMyCharacter : public ACharacter, public IInterface_Hit
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* MyInputMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* LookInputAction;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere)
	UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ShootInputAction;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* RegularMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* CooldownMaterial;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> MyProjectileClass = AProjectile::StaticClass();

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* FollowCamera;

	float ShootVelocity = 2500.f;
	FTimerHandle CooldownTimer;
	FTimerHandle RetryTimer;
	float CooldownDuration = 2.f;

	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void HitReaction(FVector HitDirection, APawn* HitInstigator) override;

	UFUNCTION(Client, Unreliable)
	void InitializeColor_Client();

	void InitializeInputMapping();
	FTransform GetShootStartTransform();
	FVector GetShootDirection(FVector StartLocation);
	void StartCooldown();
	void EndCooldown();

	void Look(const FInputActionValue& ActionValue);
	void Move(const FInputActionValue& ActionValue);
	void TryShoot();

	UFUNCTION(Server, Unreliable)
	void Shoot_Server(FTransform StartTransform, FVector Velocity);
};
