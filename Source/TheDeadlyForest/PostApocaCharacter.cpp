// Fill out your copyright notice in the Description page of Project Settings.


#include "PostApocaCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BasicZombie.h"
#include "Components/PrimitiveComponent.h"

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

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);

	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if(CharacterMesh && Gun)
	{
		Gun->AttachToComponent
		(
			CharacterMesh,
			FAttachmentTransformRules::KeepRelativeTransform,
			TEXT("RifleSocket")
		);
		Gun->SetOwner(this);
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
}

// Called to bind functionality to input
void APostApocaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Mouse
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&APostApocaCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"),this,&APostApocaCharacter::AddControllerYawInput);

	//Gamepad
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"),this,&APostApocaCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"),this,&APostApocaCharacter::LookRightRate);

	//Keyboard
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&APostApocaCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&APostApocaCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MovementMode"),this,&APostApocaCharacter::ChangeBasicMovementMode);

	//Spacebar and right trigger on the gamepad
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::Jump);

	//Aiming
	PlayerInputComponent->BindAction(TEXT("Aiming"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::StartAiming);
	PlayerInputComponent->BindAction(TEXT("Aiming"),EInputEvent::IE_Released,this,&APostApocaCharacter::EndAiming);
	PlayerInputComponent->BindAction(TEXT("ChangeCamera"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::ChangeCameraMode);

	//Shooting
	PlayerInputComponent->BindAction(TEXT("PullTrigger"),EInputEvent::IE_Pressed,this,&APostApocaCharacter::PullTrigger);
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
	if(Gun && GetMesh())
	{
		LeftHandTransform = Gun->GetMesh()->GetSocketTransform(TEXT("LeftHandSocket"));
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
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		MovementComponent->bOrientRotationToMovement = false;
		bIsPlayerAiming = true;
		bUseControllerRotationYaw= true;
		BasicMovementMode = EBasicMovementMode::Aiming;
	}
}

void APostApocaCharacter::EndAiming()
{
	if(BasicMovementMode == EBasicMovementMode::Aiming)
	{	
		UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
		MovementComponent->bOrientRotationToMovement = true;
		bIsPlayerAiming= false;
		bUseControllerRotationYaw = false;
		BasicMovementMode = EBasicMovementMode::Standing;
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

void APostApocaCharacter::PullTrigger()
{	
	bool bIsHitSomething = false;
	FHitResult HitRes;
	FVector ShotDirection;
	if(Gun && bIsPlayerAiming)
	{
		bIsHitSomething = Gun->Shoot(HitRes,ShotDirection);
	}
	if(bIsHitSomething)
	{
		if(ABasicZombie* Zombie = Cast<ABasicZombie>(HitRes.GetActor()))
		{	
			UPrimitiveComponent* HitComponent = HitRes.GetComponent();
			float Damage = Gun->GetDamage();
			//we have to check if our shot has hit enemy/zombie head
			const FName& ZombieHitBoxName = HitComponent->GetFName();
			if(ZombieHitBoxName == TEXT("HeadHitCapsule"))
			{
				Damage = Zombie->GetCurrentHealth();
			}
			AController* MyController = GetController();
			FPointDamageEvent DamageEvent(Damage,HitRes,ShotDirection,nullptr);			
			Zombie->TakeDamage(Damage,DamageEvent,MyController,Gun);		
		}			
	}
}