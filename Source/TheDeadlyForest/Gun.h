// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class USoundBase;

UENUM(BlueprintType)
enum class EGunFireMode : uint8
{
	SingleFire,
	Burst,
	FullAuto
};

UCLASS()
class THEDEADLYFOREST_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Clip")
	int ClipCapacity=40;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Clip")
	int AllAmmo = 100;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Clip")
	int CurrentAmmoInClip =0;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="FireMode")
	EGunFireMode CurrentFireMode = EGunFireMode::SingleFire;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline USkeletalMeshComponent* GetMesh() const{return GunMesh;} 
	inline float GetDamage() const {return Damage;}
	AController* GetMyOwnerController() const;
	bool FireBullet();
	bool Reload();
	void ChangeFireMode();
	inline EGunFireMode GetCurrentFireMode() const {return CurrentFireMode;}
	inline int GetMaxAmmoInBurstRound() const {return MaxAmmoInBurstRound;}

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float FireRange = 15000;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	int MaxAmmoInBurstRound = 3;

	bool GunTraceChannel(FHitResult& HitResult, FVector& ShotDirection);
	

};
