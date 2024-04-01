// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"


// Sets default values
AShield::AShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicateMovement(true);

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("<ShieldMesh>"));
	ShieldMesh->SetupAttachment(GetRootComponent());
	RootComponent = ShieldMesh;
	ShieldMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>ShieldMeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	ShieldMesh->SetStaticMesh(ShieldMeshAsset.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>ShieldMaterialAsset(TEXT("Engine.Material'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'"));
	ShieldMesh->SetMaterial(0, ShieldMaterialAsset.Object);

	ShieldMesh->SetSimulatePhysics(true);
	ShieldMesh->SetEnableGravity(false);
	ShieldMesh->SetNotifyRigidBodyCollision(true);
}

void AShield::Initialize()
{
	GetWorldTimerManager().SetTimer(AutoDestroyTimer, this, &AShield::AutoDestroy, LifeTime, false);
}

void AShield::AutoDestroy()
{
	this->Destroy();
}

void AShield::HitReaction(FVector HitDirection, APawn* HitInstigator)
{
	// Reaction to hit
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

