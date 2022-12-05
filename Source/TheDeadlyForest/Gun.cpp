// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "BasicZombie.h"

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
	Reload();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::FireBullet()
{	
	if(CurrentAmmoInClip==0)
	{
		if(!Reload())
		{
			return false;
		}
	}
	if(ShootSound)
	{
		UGameplayStatics::SpawnSoundAttached(ShootSound,GunMesh,TEXT("Muzzle"));
	}
	FHitResult HitRes;
	FVector ShotDirection;
	bool bIsHitSomething = GunTraceChannel(HitRes,ShotDirection);
	CurrentAmmoInClip--;
	if(bIsHitSomething)
	{
		if(ABasicZombie* Zombie = Cast<ABasicZombie>(HitRes.GetActor()))
		{	
			UPrimitiveComponent* HitComponent = HitRes.GetComponent();
			float ActualDamage = GetDamage();
			//we have to check if our shot has hit enemy/zombie head
			const FName& ZombieHitBoxName = HitComponent->GetFName();
			if(ZombieHitBoxName == TEXT("HeadHitCapsule"))
			{
				ActualDamage = Zombie->GetCurrentHealth();
			}
			AController* MyOwnerController = GetMyOwnerController();
			FPointDamageEvent DamageEvent(ActualDamage,HitRes,ShotDirection,nullptr);			
			Zombie->TakeDamage(ActualDamage,DamageEvent, MyOwnerController,this);		
		}			
	}
	return true;
}

bool AGun::GunTraceChannel(FHitResult& HitResult, FVector& ShotDirection)
{
	AController* Controller = GetMyOwnerController();
	FVector Location;
	FRotator Rotation;
	Controller->GetPlayerViewPoint(Location,Rotation);

	ShotDirection = -Rotation.Vector();
	FCollisionQueryParams ParamIgnore;
	ParamIgnore.AddIgnoredActor(this);
	ParamIgnore.AddIgnoredActor(GetOwner());

	FVector End = Location + Rotation.Vector() * FireRange;
	return GetWorld()->LineTraceSingleByChannel
	(
		HitResult,
		Location,
		End,
		ECC_GameTraceChannel1,
		ParamIgnore
	);
}

AController* AGun::GetMyOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return nullptr;

	AController* MyOwnerController = OwnerPawn->GetController();
	return MyOwnerController;
}

bool AGun::Reload()
{
	int AmmoToReload = ClipCapacity - CurrentAmmoInClip;
	AmmoToReload = FMath::Min(AmmoToReload,AllAmmo);
	CurrentAmmoInClip+=AmmoToReload;
	AllAmmo-=AmmoToReload;
	return AmmoToReload!=0;
}

void AGun::ChangeFireMode()
{
	switch (CurrentFireMode)
	{
		case EGunFireMode::SingleFire:
			CurrentFireMode =  EGunFireMode::Burst;
			break;
		case EGunFireMode::Burst:
			CurrentFireMode = EGunFireMode::FullAuto;
			break;
		case EGunFireMode::FullAuto:
			CurrentFireMode = EGunFireMode::SingleFire;
			break;
	}
}

