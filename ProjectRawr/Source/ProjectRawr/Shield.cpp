// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include <Kismet/GameplayStatics.h>
#include "Projectile.h"


// Sets default values
AShield::AShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<Sphere>"));
	ShieldMesh->SetupAttachment(GetRootComponent());
	RootComponent = ShieldMesh;
	ShieldMesh->SetRelativeScale3D(FVector(1.5f, 4.f, 0.15f));
	ShieldMesh->SetRelativeRotation(FRotator(90.f, 0.f, 90.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	ShieldMesh->SetStaticMesh(SphereMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>SphereMaterialAsset(TEXT("/Script/Engine.Material'/Game/ProjectRawr/Materials/M_Shield.M_Shield'"));
	ShieldMesh->SetMaterial(0, SphereMaterialAsset.Object);

	ShieldMesh->SetSimulatePhysics(false);
	ShieldMesh->SetEnableGravity(false);
	ShieldMesh->SetNotifyRigidBodyCollision(true);
}

void AShield::Initialize(APawn* NewPawnSource)
{
	PawnSource = NewPawnSource;
	
	GetWorldTimerManager().SetTimer(AutoDestroyTimer, this, &AShield::AutoDestroy, LifeTime, false);
}

void AShield::AutoDestroy()
{
	this->Destroy();
}

void AShield::HitReaction(FVector HitDirection, APawn* HitInstigator)
{
	// Reaction to hit
	
	GetWorldTimerManager().ClearTimer(AutoDestroyTimer);
	AutoDestroy();
}


void AShield::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
}

