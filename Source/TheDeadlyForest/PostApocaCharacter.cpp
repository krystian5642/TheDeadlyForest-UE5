// Fill out your copyright notice in the Description page of Project Settings.


#include "PostApocaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasicZombie.h"
#include "Components/PrimitiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "KillAndSurviveGameMode.h"

// Sets default values
APostApocaCharacter::APostApocaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    ThirdPersonCamera  = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if(CharacterMesh)
	{
		SpringArm->SetupAttachment(CharacterMesh);
		FirstPersonCamera->AttachToComponent
		(
			CharacterMesh,
			FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("head")
		);
		FirstPersonCamera->bAutoActivate =false;
    	ThirdPersonCamera->SetupAttachment(SpringArm);
	}
}

// Called when the game starts or when spawned
void APostApocaCharacter::BeginPlay()
{
	Super::BeginPlay();

	WeaponsNumber = WeaponClasses.Num();
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if(CharacterMesh)
	{
		for(const auto& WeaponClass : WeaponClasses)
		{
			if(WeaponClass)
			{
				AllAvailableWeapons.Add(GetWorld()->SpawnActor<AGun>(WeaponClass));
				if(AGun* TheLastAddedGun = AllAvailableWeapons.Top())
				{
					TheLastAddedGun->AttachToComponent
					(
						CharacterMesh,
						FAttachmentTransformRules::KeepRelativeTransform,
						TEXT("WeaponSocket")
					);
					TheLastAddedGun->SetOwner(this);
					TheLastAddedGun->SetActorHiddenInGame(true);
				}
			}
			else
			{
				AllAvailableWeapons.Add(nullptr);
			}
		}
		CurrentWeapon = AllAvailableWeapons[0];
		if(CurrentWeapon)
		{
			CurrentWeapon->SetActorHiddenInGame(false);
		}
	}
	CurrentHealth=MaxHealth;
}

// Called every frame
void APostApocaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AdjustMaxWalkSpeed();
	UpdateLeftHandTransform();
	ControlCameraMode();
	UseWeaponIfAttack();
}

// Called to bind functionality to input
void APostApocaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Basic Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&APostApocaCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&APostApocaCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MovementMode"),this,&APostApocaCharacter::ChangeBasicMovementMode);
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::Jump);

	//Combat
	PlayerInputComponent->BindAction(TEXT("SwitchWeaponUp"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::SwitchWeaponUp);
	PlayerInputComponent->BindAction(TEXT("SwitchWeaponDown"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::SwitchWeaponDown);
	PlayerInputComponent->BindAction(TEXT("ReloadGun"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::TryToReload);
	PlayerInputComponent->BindAction(TEXT("Aiming"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::StartAiming);
	PlayerInputComponent->BindAction(TEXT("Aiming"),EInputEvent::IE_Released,this,&APostApocaCharacter::EndAiming);
	PlayerInputComponent->BindAction(TEXT("Attack"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::TryToAttack);
	PlayerInputComponent->BindAction(TEXT("Attack"),EInputEvent::IE_Released,this,&APostApocaCharacter::TryToAttack);
	PlayerInputComponent->BindAction(TEXT("ChangeFireMode"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::ChangeFireMode);

	//Camera rotation, change
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&APostApocaCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"),this,&APostApocaCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"),this,&APostApocaCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"),this,&APostApocaCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("ChangeCamera"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::ChangeCameraMode);
}

float APostApocaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	UE_LOG(LogTemp,Display,TEXT("%f"),CurrentHealth);
	if(DamageAmount<=0 || CurrentHealth == 0)
	{
		return 0;
	}
	float DamageApplied = FMath::Min(CurrentHealth,DamageAmount);
	CurrentHealth-=DamageApplied;
	if(!IsAlive())
	{
		AKillAndSurviveGameMode* GameMode = GetWorld()->GetAuthGameMode<AKillAndSurviveGameMode>();
		if(GameMode)
		{
			PlayerHasDied();		
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer
			(
				UnusedHandle, 
				this, 
				&APostApocaCharacter::DropWeapon, 
				0.8, 
				false
			);		
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageApplied;
}


void APostApocaCharacter::MoveForward(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = UKismetMathLibrary::GetForwardVector(YawRotation);
		AddMovementInput(Direction, AxisValue);
	}
}

void APostApocaCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = UKismetMathLibrary::GetRightVector(YawRotation);
		AddMovementInput(Direction, AxisValue);
	}
}

void APostApocaCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * ControllerRotationRate * GetWorld()->GetDeltaSeconds());
}

void APostApocaCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * ControllerRotationRate * GetWorld()->GetDeltaSeconds());
}

void APostApocaCharacter::ChangeBasicMovementMode(float AxisValue)
{
	const FVector Velocity = GetVelocity();
	float GroundSpeed = FVector(Velocity.X,Velocity.Y,0).Size();
	if(BasicMovementMode == EBasicMovementMode::Aiming)
	{
		return;
	}
	if(GroundSpeed == 0 && BasicMovementMode != EBasicMovementMode::Standing)
	{
		BasicMovementMode = EBasicMovementMode::Standing;
	}
	else if(BasicMovementMode == EBasicMovementMode::Standing && GroundSpeed)
	{
		BasicMovementMode = EBasicMovementMode::Walking;
	}
	else if(BasicMovementMode == EBasicMovementMode::Walking && AxisValue)
	{
		BasicMovementMode = EBasicMovementMode::Running;
	}
	else if(BasicMovementMode == EBasicMovementMode::Running && !AxisValue)
	{
		BasicMovementMode = EBasicMovementMode::Walking;
	}
}

void APostApocaCharacter::Jump()
{
	if( BasicMovementMode==EBasicMovementMode::Running)
	{
		Super::Jump();
	}
}

void APostApocaCharacter::UpdateLeftHandTransform()
{
	if(CurrentWeapon && GetMesh())
	{
		LeftHandTransform = CurrentWeapon->GetMesh()->GetSocketTransform(TEXT("LeftHandSocket"));
		FVector OutPosition = LeftHandTransform.GetLocation();
		FRotator OutRotation = LeftHandTransform.GetRotation().Rotator();
		GetMesh()->TransformToBoneSpace
		(
    		TEXT("hand_r"),
    		OutPosition,
    		OutRotation,
    		OutPosition,
    		OutRotation
		);
		LeftHandTransform = FTransform(OutRotation.Quaternion(),OutPosition);
	}
}

void APostApocaCharacter::StartAiming()
{
	if(HasAGun())
	{
		SetAiming(true);
		BasicMovementMode = EBasicMovementMode::Aiming;
	}
}

void APostApocaCharacter::EndAiming()
{
	if(BasicMovementMode == EBasicMovementMode::Aiming)
	{	
		SetAiming(false);
		BasicMovementMode = EBasicMovementMode::Standing;
	}
}

void APostApocaCharacter::SetAiming(bool bIsAiming)
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if(!MovementComponent)
	{
		return;
	}
	MovementComponent->bOrientRotationToMovement = !bIsAiming;
	bIsPlayerAiming= bIsAiming;
	bUseControllerRotationYaw = bIsAiming;
}

void APostApocaCharacter::SwitchWeaponUp()
{
	++CurrentWeaponIndex%=WeaponsNumber;
	EnableNewWeapon();
}

void APostApocaCharacter::SwitchWeaponDown()
{
	if(CurrentWeaponIndex - 1 < 0)
	{
		CurrentWeaponIndex = WeaponsNumber-1;
	}
	else 
	{
		CurrentWeaponIndex--;
	}
	EnableNewWeapon();
}

void APostApocaCharacter::EnableNewWeapon()
{
	if(CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}
	CurrentWeapon = AllAvailableWeapons[CurrentWeaponIndex];
	if(CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(false);
	}
	else if(bIsPlayerAiming)
	{
		bIsPlayerAiming = false;
	}
}

void APostApocaCharacter::ChangeCameraMode()
{
	switch(CameraMode)
	{
		case ECameraMode::ThirdPersonClose:
			CameraMode = ECameraMode::ThirdPersonFar;
			SpringArm->TargetArmLength*=3.f/2.f;
			break;

		case ECameraMode::ThirdPersonFar:
			ToFirstPersonCamera(true);
			break;

		case ECameraMode::FirstPerson:
			SpringArm->TargetArmLength*=2.f/3.f;			
			ToFirstPersonCamera(false);
			break;
	}
}

void APostApocaCharacter::ToFirstPersonCamera(bool bFirstPerson)
{
	bUseControllerRotationYaw=bFirstPerson;
	FirstPersonCamera->SetActive(bFirstPerson);
	ThirdPersonCamera->SetActive(!bFirstPerson);
	if(bFirstPerson)
	{
		CameraMode = ECameraMode::FirstPerson;
	}
	else
	{
		CameraMode = ECameraMode::ThirdPersonClose;
	}
	
}

void APostApocaCharacter::ControlCameraMode()
{
	if(CameraMode == ECameraMode::FirstPerson && bUseControllerRotationYaw== false)
	{
		bUseControllerRotationYaw= true;
	}
}

void APostApocaCharacter::DropWeapon()
{
	if(CurrentWeapon && CurrentWeapon->GetMesh())
	{
		CurrentWeapon->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		CurrentWeapon->GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));
		CurrentWeapon->GetMesh()->SetSimulatePhysics(true);
	}
}

void APostApocaCharacter::TryToAttack()
{
	bIsTryingToAttack=!bIsTryingToAttack;
	AmmoRounds = 0;
}

void APostApocaCharacter::TryToReload()
{
	if(AGun* GunToReload = Cast<AGun>(CurrentWeapon))
	{
		GunToReload->Reload();
	}
}

void APostApocaCharacter::UseWeaponIfAttack()
{
	if(bIsTryingToAttack && CurrentWeapon)
	{
		EGunFireMode CurrentFireMode = CurrentWeapon->GetCurrentFireMode();
		if(CurrentFireMode == EGunFireMode::SingleFire && AmmoRounds == 0)
		{
			if(CurrentWeapon->FireBullet())
			{
				AmmoRounds++;
			}
		}
		else if(CurrentFireMode == EGunFireMode::Burst && AmmoRounds<CurrentWeapon->GetMaxAmmoInBurstRound())
		{
			if(CurrentWeapon->FireBullet())
			{
				AmmoRounds++;
			}
		}
		else if(CurrentFireMode == EGunFireMode::FullAuto)
		{
			if(CurrentWeapon->FireBullet())
			{
				AmmoRounds++;
			}
		}
	}
}

void APostApocaCharacter::ChangeFireMode()
{
	if(Cast<AGun>(CurrentWeapon))
	{
		CurrentWeapon->ChangeFireMode();
	}
}