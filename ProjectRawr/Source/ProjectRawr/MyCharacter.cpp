// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);
	GetCapsuleComponent()->SetCapsuleRadius(35.f);

	// Mesh
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	CameraBoom->SetRelativeLocation(FVector(0.f, 0.f, 8.5f));
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SocketOffset = FVector(0.f, 40.f, 70.f);
	CameraBoom->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;

	// Character movement
	UCharacterMovementComponent* CharMovComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharMovComp != nullptr)
	{
		CharMovComp->GravityScale = 1.75f;
		CharMovComp->MaxAcceleration = 1500.f;
		CharMovComp->BrakingFrictionFactor = 1.f;
		CharMovComp->bUseSeparateBrakingFriction = true;
		CharMovComp->MaxWalkSpeed = 500.f;
		CharMovComp->MinAnalogWalkSpeed = 20.f;
		CharMovComp->BrakingDecelerationWalking = 2000.f;
		CharMovComp->JumpZVelocity = 700.f;
		CharMovComp->BrakingDecelerationFalling = 1500.f;
		CharMovComp->AirControl = 0.35f;
		CharMovComp->RotationRate = FRotator(0.f, 500.f, 0.f);
		CharMovComp->bUseControllerDesiredRotation = false;
		CharMovComp->bOrientRotationToMovement = true;
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeInputMapping();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* MyEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	MyEnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
	MyEnhancedInputComponent->BindAction(ShootInputAction, ETriggerEvent::Started, this, &AMyCharacter::TryShoot);
	MyEnhancedInputComponent->BindAction(ShieldInputAction, ETriggerEvent::Started, this, &AMyCharacter::TryShield);
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(NewController);
	if (MyPlayerController)
		InitializeColor_Client();
}

void AMyCharacter::HitReaction(FVector HitDirection, APawn* HitInstigator)
{
	//FVector LaunchVelocity = HitDirection * 500.f + FVector(0.f, 0.f, 500.f);
	//LaunchCharacter(LaunchVelocity, false, false);
}

void AMyCharacter::InitializeColor_Client_Implementation()
{
	GetMesh()->SetMaterial(1, RegularMaterial);
}

void AMyCharacter::InitializeInputMapping()
{
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetController());
	if (MyPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyPlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(MyInputMappingContext, 0);
		}
	}
	else GetWorldTimerManager().SetTimer(RetryTimer, this, &AMyCharacter::InitializeInputMapping, 0.2f, false);
}

FTransform AMyCharacter::GetShootStartTransform()
{
	FRotator Rotation = GetActorRotation();
	FVector Scale = FVector(1.f, 1.f, 1.f);

	FVector Location = GetActorLocation();
	FVector CameraHorizontalForward = FollowCamera->GetForwardVector();
	CameraHorizontalForward.Z = 0.f;
	Location += CameraHorizontalForward.GetSafeNormal(0.0001f) * 100.f;
	Location += FVector(0.f, 0.f, 50.f);

	return FTransform(Rotation, Location, Scale);
}

FVector AMyCharacter::GetShootDirection(FVector StartLocation)
{
	FVector LocalStart = FollowCamera->GetComponentLocation();
	FVector LocalEnd = LocalStart + FollowCamera->GetForwardVector() * 10000.f;

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, LocalStart, LocalEnd, ECC_WorldStatic, QueryParams);

	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
		return (Hit.Location - StartLocation).GetSafeNormal(0.0001f);
	else
		return (LocalEnd - StartLocation).GetSafeNormal(0.0001f);
}

void AMyCharacter::StartShootCooldown()
{
	GetMesh()->SetMaterial(1, CooldownMaterial);
	GetWorldTimerManager().SetTimer(CooldownTimer, this, &AMyCharacter::EndShootCooldown, CooldownDuration, false);
}

void AMyCharacter::EndShootCooldown()
{
	GetMesh()->SetMaterial(1, RegularMaterial);
}

void AMyCharacter::StartShieldCooldown()
{
	GetMesh()->SetMaterial(0, ShieldCooldownMaterial);
	GetWorldTimerManager().SetTimer(ShieldCooldownTimer, this, &AMyCharacter::EndShieldCooldown, ShieldCooldownDuration, false);
}

void AMyCharacter::EndShieldCooldown()
{
	GetMesh()->SetMaterial(0, RegularMaterial);
}

void AMyCharacter::Look(const FInputActionValue& ActionValue)
{
	const FVector2D LookValue = ActionValue.Get<FVector2D>();
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void AMyCharacter::TryShoot()
{
	if (GetWorldTimerManager().GetTimerRemaining(CooldownTimer) <= 0.f)
	{
		FTransform StartTransform = GetShootStartTransform();
		FVector Velocity = GetShootDirection(StartTransform.GetLocation()) * ShootVelocity;
		StartTransform.SetLocation(StartTransform.GetLocation() + FollowCamera->GetForwardVector() * 5);
		Shoot_Server(StartTransform, Velocity);
		StartShootCooldown();
	}
}

void AMyCharacter::TryShield()
{
	if (GetWorldTimerManager().GetTimerRemaining(ShieldCooldownTimer) <= 0.f)
	{
		FTransform StartTransform = GetShootStartTransform();
		FVector Velocity = GetShootDirection(StartTransform.GetLocation()) * ShootVelocity;

		Shield_Server(StartTransform, Velocity);
		StartShieldCooldown();
	}
}

void AMyCharacter::Shield_Server_Implementation(FTransform StartTransform, FVector Velocity)
{
	FVector LocalSpawnLocation = StartTransform.GetLocation();
	AShield* MyShield = Cast<AShield>(GetWorld()->SpawnActor(MyShieldClass, &LocalSpawnLocation));
	if (MyShield != nullptr)
		MyShield->Initialize();
}


void AMyCharacter::Shoot_Server_Implementation(FTransform StartTransform, FVector Velocity)
{
		FVector LocalSpawnLocation = StartTransform.GetLocation();
		AProjectile* MyProjectile = Cast<AProjectile>(GetWorld()->SpawnActor(MyProjectileClass, &LocalSpawnLocation));
		if (MyProjectile != nullptr)
			MyProjectile->Initialize(Velocity, this);
}
