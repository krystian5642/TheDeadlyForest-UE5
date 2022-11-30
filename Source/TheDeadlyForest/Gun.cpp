// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	SetRootComponent(GunMesh);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo=MaxAmmo;
}

bool AGun::Shoot(FHitResult& HitResult, FVector& ShotDirection)
{	
	if(CurrentAmmo==0)
	{
		return false;
	}
	if(ShootSound)
	{
		UGameplayStatics::SpawnSoundAttached(ShootSound,GunMesh,TEXT("Muzzle"));
	}

	AController* Controller = GetMyOwnerController();
	FVector Location;
	FRotator Rotation;
	Controller->GetPlayerViewPoint(Location,Rotation);

	FCollisionQueryParams ParamIgnore;
	ParamIgnore.AddIgnoredActor(this);
	ParamIgnore.AddIgnoredActor(GetOwner());

	FVector End = Location + Rotation.Vector() * 1000;
	bool bHitSomething = GetWorld()->LineTraceSingleByChannel
	(
		HitResult,
		Location,
		End,
		ECC_GameTraceChannel1,
		ParamIgnore
	);
	CurrentAmmo--;
	return bHitSomething;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AController* AGun::GetMyOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return nullptr;

	AController* MyOwnerController = OwnerPawn->GetController();
	return MyOwnerController;
}

