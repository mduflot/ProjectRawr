// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Interface_Hit.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->Sphere->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FTransform SpawnTransform = FTransform(FRotator(0.f, 0.f, 0.f), GetActorLocation(), FVector(1.f, 1.f, 1.f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEmitter, SpawnTransform, true, EPSCPoolMethod::None,
	                                         true);
}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()))
	{
		IInterface_Hit* CastedOther = Cast<IInterface_Hit>(OtherActor);
		if (CastedOther)
		{
			CastedOther->HitReaction(ThrowVelocity.GetSafeNormal(0.0001), PawnSource);
		}
		AutoDestroy();
	}
}

void AProjectile::Initialize(FVector NewVelocity, APawn* NewPawnSource)
{
	PawnSource = NewPawnSource;
	ThrowVelocity = NewVelocity;
	Sphere->SetPhysicsLinearVelocity(ThrowVelocity, false);
	GetWorldTimerManager().SetTimer(AutoDestroyTimer, this, &AProjectile::AutoDestroy, LifeTime, false);
}

void AProjectile::AutoDestroy()
{
	this->Destroy();
}
