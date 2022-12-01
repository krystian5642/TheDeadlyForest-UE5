// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class USoundBase;

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
	int MaxAmmo = 40;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Clip")
	int CurrentAmmo =0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline USkeletalMeshComponent* GetMesh() const{return GunMesh;} 
	bool Shoot(FHitResult& HitResult, FVector& ShotDirection);

	inline float GetDamage() const {return Damage;}

	AController* GetMyOwnerController() const;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float Damage = 20.f;

	UPROPERTY(EditDefaultsOnly,Category="Combat")
	float FireRange = 15000;
};
