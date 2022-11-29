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

	UPROPERTY(EditDefaultsOnly,Category="Clip")
	int16 MaxAmmo = 40;
	
	UPROPERTY(VisibleAnywhere,Category="Clip")
	int16 CurrentAmmo =0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	inline USkeletalMeshComponent* GetMesh() const{return GunMesh;} 
	bool Shoot();

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;

};
