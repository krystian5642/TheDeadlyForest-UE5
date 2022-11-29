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

bool AGun::Shoot()
{	
	if(CurrentAmmo==0)
	{
		return false;
	}
	if(ShootSound)
	{
		UGameplayStatics::SpawnSoundAttached(ShootSound,GunMesh,TEXT("Muzzle"));
	}
	CurrentAmmo--;
	return true;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

