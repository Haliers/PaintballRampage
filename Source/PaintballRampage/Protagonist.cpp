// Fill out your copyright notice in the Description page of Project Settings.


#include "Protagonist.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Shotgun.h"
#include "Engine/EngineTypes.h"
#include "Components/SphereComponent.h"
#include "AmmoDrop.h"
#include "Library.h"


//#include "Shotgun.h"
//#include "M4.h"
//#include "Glock.h"

// Sets default values
AProtagonist::AProtagonist() :
	Speed(100.f),
	LookSensitivity(1.f),
	// Reserve ammo defaults
	PurpleAmmoReserve(10.f),
	YellowAmmoReserve(10.f),
	GreenAmmoReserve(10.f),
	BlueAmmoReserve(10.f),
	RedAmmoReserve(10.f),
	KeyText(TEXT("E")),
	CurrentWeaponIndex(0),
	KillMilestoneStart(10),
	Milestone(1),
	MilestoneKillCounterSwitch(false),
	CounterText(TEXT("Next milestone:")),
	bCanJump(true),
	JumpCoolDown(5.f),
	JumpCoolDownCounter(0.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->DestroyComponent();
	GetMesh()->SetActive(false);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(RootComponent);
	CameraComponent->bUsePawnControlRotation = true;

	GunSceneComponent = CreateDefaultSubobject<USceneComponent>("GunSceneComponent");
	GunSceneComponent->SetupAttachment(CameraComponent);

	DamageSphere = CreateDefaultSubobject<USphereComponent>("DamageSphere");

	LineTraceSceneComponent = CreateDefaultSubobject<USceneComponent>("LineTraceSceneComponent");
	LineTraceSceneComponent->SetupAttachment(CameraComponent);
}

// Called when the game starts or when spawned
void AProtagonist::BeginPlay()
{
	Super::BeginPlay();
	
	HP = StartingHP;
	CharacterMovementComponent = GetCharacterMovement();

	BeginPlaySetup();
}

void AProtagonist::MoveForward(float Val)
{
	AddMovementInput(GetActorForwardVector() * Val * Speed);
}

void AProtagonist::MoveRight(float Val)
{
	AddMovementInput(GetActorRightVector() * Val * Speed);
}

void AProtagonist::LookVertical(float Val)
{
	AddControllerPitchInput(Val * LookSensitivity);
}

void AProtagonist::LookHorizontal(float Val)
{
	AddControllerYawInput(Val * LookSensitivity);
}

void AProtagonist::BeginPlaySetup()
{
	Weapons.Empty();

	for (auto WeaponClass : WeaponClasses)
	{
		Weapons.Add(SpawnWeapon(WeaponClass));
		Weapons.Last()->SetActorLocation(FVector(0.f, 0.f, -200.f));
	}
	WeaponsAvailable.Add(Weapons[0]);

	EquipWeaponByCurrentIndex();

	KillMilestone = KillMilestoneStart;

	JumpCoolDownCounter = JumpCoolDown;
}

void AProtagonist::GetHitResultFromLineTraceComponent()
{
	if (LineTraceSceneComponent && WeaponsAvailable[CurrentWeaponIndex])
	{
		FVector TraceBegin{ LineTraceSceneComponent->GetComponentLocation() };
		FVector TraceEnd{ TraceBegin + (LineTraceSceneComponent->GetComponentRotation().Vector() * 10000) };
		GetWorld()->LineTraceSingleByChannel(HitUnderLineTraceComponent, TraceBegin, TraceEnd, ECollisionChannel::ECC_GameTraceChannel3);

		if (!HitUnderLineTraceComponent.bBlockingHit)
		{
			HitUnderLineTraceComponent.Location = HitUnderLineTraceComponent.TraceEnd;
		}

		if (WeaponsAvailable[CurrentWeaponIndex])
		{
			WeaponsAvailable[CurrentWeaponIndex]->SetHitResultFromCharacter(HitUnderLineTraceComponent);
		}

		// DrawDebugSphere(GetWorld(), TraceBegin, 25.f, 8.f, FColor::Yellow);
		// DrawDebugSphere(GetWorld(), TraceEnd, 25.f, 8.f, FColor::Blue);
		// DrawDebugSphere(GetWorld(), HitUnderCamera.Location, 25.f, 8.f, FColor::Red);
	}
}

void AProtagonist::JumpAbility()
{
	if (!CharacterMovementComponent->IsFalling() && bCanJump)
	{
		CharacterMovementComponent->AddImpulse(FVector(0.f,0.f,1000.f), true);

		bCanJump = false;
		JumpCoolDownCounter = 0;
	}	
}

void AProtagonist::JumpCounter(float DeltaTime)
{
	JumpCoolDownCounter += DeltaTime;
	JumpCoolDownPercentage = JumpCoolDownCounter / JumpCoolDown;
	if (JumpCoolDownPercentage >= 1 && !bCanJump)
	{
		JumpCoolDownPercentage = 1;
		bCanJump = true;
	}
}

void AProtagonist::PullTrigger()
{
	if (WeaponsAvailable[CurrentWeaponIndex]) 
	{
		WeaponsAvailable[CurrentWeaponIndex]->SetTriggerPulled(true);
		WeaponsAvailable[CurrentWeaponIndex]->InitiateFireSequence();
	}
}

void AProtagonist::ReleaseTrigger()
{
	if (WeaponsAvailable[CurrentWeaponIndex]) {
		WeaponsAvailable[CurrentWeaponIndex]->SetTriggerPulled(false);
	}
}

void AProtagonist::InitiateReloadFromCharacter()
{
	if (WeaponsAvailable[CurrentWeaponIndex] && !WeaponsAvailable[CurrentWeaponIndex]->GetIsReloading() && WeaponsAvailable[CurrentWeaponIndex]->GetCanFire())
	{
		float EmptyAmmo = WeaponsAvailable[CurrentWeaponIndex]->GetMagSize() - WeaponsAvailable[CurrentWeaponIndex]->GetAmmo();

		if (EmptyAmmo > 0) {
			float ReserveToBeChecked{ 0 };
			switch (WeaponsAvailable[CurrentWeaponIndex]->GetAmmoColor())
			{
			case EAmmoColor::EAC_Purple:
				ReserveToBeChecked = PurpleAmmoReserve;
				break;
			case EAmmoColor::EAC_Yellow:
				ReserveToBeChecked = YellowAmmoReserve;
				break;
			case EAmmoColor::EAC_Green:
				ReserveToBeChecked = GreenAmmoReserve;
				break;
			case EAmmoColor::EAC_Blue:
				ReserveToBeChecked = BlueAmmoReserve;
				break;
			case EAmmoColor::EAC_Red:
				ReserveToBeChecked = RedAmmoReserve;
				break;
			}
			if (ReserveToBeChecked > 0)
			{
				WeaponsAvailable[CurrentWeaponIndex]->InitiateReload();
			}
		}
	}
}

void AProtagonist::FinaliseReloadFromCharacter()
{
	if (WeaponsAvailable[CurrentWeaponIndex]) {
		float EmptyAmmo = WeaponsAvailable[CurrentWeaponIndex]->GetMagSize() - WeaponsAvailable[CurrentWeaponIndex]->GetAmmo();

			
		float NewReserve = WeaponsAvailable[CurrentWeaponIndex]->FillMag(
			GetAmmoReserveForAmmoColor(),
			EmptyAmmo);
		SetAmmoReserveForAmmoColor(NewReserve);

		WeaponsAvailable[CurrentWeaponIndex]->SetReloading(false);
		WeaponsAvailable[CurrentWeaponIndex]->CalculateCanFire();
	}
}

AWeapon* AProtagonist::SpawnWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	if (WeaponClass) {
		return GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	}
	return nullptr;
}

void AProtagonist::EquipWeaponByCurrentIndex()
{
	int32 PreviousIndex = CurrentWeaponIndex - 1;

	if (PreviousIndex < 0)
	{
		PreviousIndex = WeaponsAvailable.Num() - 1;
	}
	if (WeaponsAvailable[PreviousIndex])
	{
		const FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
		WeaponsAvailable[PreviousIndex]->DetachFromActor(DetachmentRules);
		WeaponsAvailable[PreviousIndex]->SetActorLocation(FVector(0.f, 0.f, -200.f));
	}
	
	if (WeaponsAvailable[CurrentWeaponIndex])
	{
		FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(
			EAttachmentRule::KeepRelative,
			false);

		WeaponsAvailable[CurrentWeaponIndex]->AttachToComponent(
			GunSceneComponent,
			AttachmentRules);

		WeaponsAvailable[CurrentWeaponIndex]->SetActorRelativeLocation(FVector(0.f, 0.f, 0.f));
		WeaponsAvailable[CurrentWeaponIndex]->SetActorRelativeRotation(FRotator(0.f, 0.f, 0.f));

		WeaponsAvailable[CurrentWeaponIndex]->SetChangingWeapon(true);

		WeaponsAvailable[CurrentWeaponIndex]->PlaySwapInAnimation();
	}
}

void AProtagonist::CockWeapon()
{
	if (WeaponsAvailable[CurrentWeaponIndex])
	{
		WeaponsAvailable[CurrentWeaponIndex]->SetCocked(true);
	}
}

void AProtagonist::SetWeaponMovement()
{
	if (WeaponsAvailable[CurrentWeaponIndex])
	{
		if (GetVelocity().Length() != 0)
		{
			WeaponsAvailable[CurrentWeaponIndex]->SetIfIdle(false);
		}
		else
		{
			WeaponsAvailable[CurrentWeaponIndex]->SetIfIdle(true);
		}
	}
}

int32 AProtagonist::GetAmmoReserveForAmmoColor()
{
	
	if (WeaponsAvailable[CurrentWeaponIndex])
	{
		int32 Reserve = 0.f;

		EAmmoColor WeaponAmmoColor = WeaponsAvailable[CurrentWeaponIndex]->GetAmmoColor();

			switch (WeaponAmmoColor)
			{
			case EAmmoColor::EAC_Purple:
				Reserve = PurpleAmmoReserve;
				break;
			case EAmmoColor::EAC_Yellow:
				Reserve = YellowAmmoReserve;
				break;
			case EAmmoColor::EAC_Green:
				Reserve = GreenAmmoReserve;
				break;
			case EAmmoColor::EAC_Blue:
				Reserve = BlueAmmoReserve;
				break;
			case EAmmoColor::EAC_Red:
				Reserve = RedAmmoReserve;
				break;
			default:
				break;
			}

		return Reserve;
	}

	return 0.f;
}

void AProtagonist::SetAmmoReserveForAmmoColor(float Reserve)
{
	if (WeaponsAvailable[CurrentWeaponIndex])
	{
		EAmmoColor WeaponAmmoColor = WeaponsAvailable[CurrentWeaponIndex]->GetAmmoColor();

		switch (WeaponAmmoColor)
		{
		case EAmmoColor::EAC_Purple:
			PurpleAmmoReserve = Reserve;
			break;
		case EAmmoColor::EAC_Yellow:
			YellowAmmoReserve = Reserve;
			break;
		case EAmmoColor::EAC_Green:
			GreenAmmoReserve = Reserve;
			break;
		case EAmmoColor::EAC_Blue:
			BlueAmmoReserve = Reserve;
			break;
		case EAmmoColor::EAC_Red:
			RedAmmoReserve = Reserve;
			break;
		default:
			break;
		}
	}
}

void AProtagonist::ChangeWeapon()
{
	if (WeaponsAvailable.Num() > 1)
	{
		CurrentWeaponIndex++;
		if (CurrentWeaponIndex >= WeaponsAvailable.Num()) CurrentWeaponIndex = 0;

		EquipWeaponByCurrentIndex();
	}
}

void AProtagonist::DeathSetupBlueprint_Implementation()
{
}

// Called every frame
void AProtagonist::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetWeaponMovement();
	GetHitResultFromLineTraceComponent();
	JumpCounter(DeltaTime);

	// DrawDebugSphere(GetWorld(), HitUnderLineTraceComponent.Location, 25.f, 8.f, FColor::Red, true);
}

AWeapon* AProtagonist::GetEquippedWeapon()
{
	if (WeaponsAvailable[CurrentWeaponIndex]) {
		return WeaponsAvailable[CurrentWeaponIndex];
	}

	return nullptr;
}

void AProtagonist::ApplyDamage(float Damage)
{
	HP -= Damage;
	
	if (HP <= 0)
	{
		Die();
	}

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("HP: %f"), HP));
}

void AProtagonist::Die()
{
	DisableInput(Cast<APlayerController>(GetController()));

	if (WeaponsAvailable[CurrentWeaponIndex])
	{
		FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(
			EDetachmentRule::KeepWorld, 
			EDetachmentRule::KeepWorld, 
			EDetachmentRule::KeepWorld, 
			false);
		WeaponsAvailable[CurrentWeaponIndex]->Destroy();

		DeathSetupBlueprint();
	}
}

UAnimInstance* AProtagonist::GetAnimInstance()
{
	if (GetMesh())
	{
		return GetMesh()->GetAnimInstance();
	}
	else
	{
		return nullptr;
	}
}

int32 AProtagonist::GetCurrentReserveAmmo()
{
	EAmmoColor CurrentAmmoColor = WeaponsAvailable[CurrentWeaponIndex]->GetAmmoColor();
	int32 CurrentAmmoReserve = 0;

	switch (CurrentAmmoColor)
	{
	case EAmmoColor::EAC_Purple:
		CurrentAmmoReserve = PurpleAmmoReserve;
		break;
	case EAmmoColor::EAC_Yellow:
		CurrentAmmoReserve = YellowAmmoReserve;
		break;
	case EAmmoColor::EAC_Green:
		CurrentAmmoReserve = GreenAmmoReserve;
		break;
	case EAmmoColor::EAC_Blue:
		CurrentAmmoReserve = BlueAmmoReserve;
		break;
	case EAmmoColor::EAC_Red:
		CurrentAmmoReserve = RedAmmoReserve;
		break;
	default:
		break;
	}

	return CurrentAmmoReserve;
}

void AProtagonist::ProgressMilestone()
{
	if (!MilestoneKillCounterSwitch)
	{
		KillMilestone--;

		if (KillMilestone == 0)
		{
			int32 MaxMilestone = Weapons.Num();

			if (Milestone < MaxMilestone)
			{
				WeaponsAvailable.Add(Weapons[Milestone]);
				Milestone++;
			}
			else
			{
				MilestoneKillCounterSwitch = true;
				KillMilestone = 1;
				CounterText = TEXT("Kills:");
				return;
			}

			KillMilestone = KillMilestoneStart;
		}
	}
	else
	{
		KillMilestone++;
	}
}

// Called to bind functionality to input
void AProtagonist::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProtagonist::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProtagonist::MoveRight);
	PlayerInputComponent->BindAxis("LookVertical", this, &AProtagonist::LookVertical);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &AProtagonist::LookHorizontal);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AProtagonist::PullTrigger);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AProtagonist::ReleaseTrigger);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AProtagonist::InitiateReloadFromCharacter);
	PlayerInputComponent->BindAction("Special", IE_Pressed, this, &AProtagonist::JumpAbility);
	PlayerInputComponent->BindAction("Weapons", IE_Pressed, this, &AProtagonist::ChangeWeapon);

}

