// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "TimerManager.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.h"
#include "Math/Color.h"
#include "Library.h"


// Sets default values
AWeapon::AWeapon() :
	MagSize(12.f),
	bTriggerPulled(false),
	bIdle(true),
	bCanFire(true),
	bCocked(true),
	bChangingWeapon(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);

	MuzzleSceneComponent = CreateDefaultSubobject<USceneComponent>("MuzzleSceneComponent");
	MuzzleSceneComponent->SetupAttachment(SkeletalMeshComponent);

	FireAnimMontage = CreateDefaultSubobject<UAnimMontage>("FireAnimation");
	ReloadAnimMontage = CreateDefaultSubobject<UAnimMontage>("ReloadAnimation");

	DecalSize = FVector(10.f,50.f,50.f);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	BeginPlaySetup();
}

void AWeapon::ApplyDamageIfDamagable(FHitResult Param_Hit)
{
	ABaseEnemy* BaseEnemyRef = Cast<ABaseEnemy>(Param_Hit.GetActor());

	if (BaseEnemyRef)
	{
		// ONSCREEN_DEBUG("Hit actor is Damagable", 1)
		BaseEnemyRef->ApplyDamage(Damage);
	}
	else
	{
		// ONSCREEN_DEBUG("Hit actor not Damagable", 1)
	}
}

void AWeapon::SpawnTracer_Implementation()
{
}

void AWeapon::SpawnImpactFX_Implementation(FVector ImpactLocation, FVector ImpactNormal)
{
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::SetChangingWeapon(bool Param_bChaningWeapon)
{
	bChangingWeapon = Param_bChaningWeapon;
	CalculateCanFire();

	//if (bChangingWeapon)
	//{
	//	ONSCREEN_DEBUG("TRUE", 1)
	//}
	//else
	//{
	//	ONSCREEN_DEBUG("FALSE", 1)
	//}
}

bool AWeapon::CalculateCanFire()
{
	if (!bReloading && bCocked && !bChangingWeapon)
	{
		bCanFire = true;
	}
	else
	{
		bCanFire = false;
	}

	return bCanFire;
}

void AWeapon::InitiateReload()
{
	bReloading = true;
	CalculateCanFire();

	UAnimInstance* AnimInstance{ (SkeletalMeshComponent) ? SkeletalMeshComponent->GetAnimInstance() : nullptr };
	if (AnimInstance && ReloadAnimMontage)
	{
		AnimInstance->Montage_Play(ReloadAnimMontage);
	}
}

int32 AWeapon::FillMag(int32 Fill, int32 Empty)
{
	int32 ExtraAmmo = 0; 

	AmmoInMag += Fill;

	if (AmmoInMag > MagSize)
	{
		ExtraAmmo = AmmoInMag - MagSize;
	
		AmmoInMag = MagSize;
	}
	
	return ExtraAmmo;
}

int32 AWeapon::RemoveAmmoInMag()
{
	int32 ReturnedAmount{ AmmoInMag };
	AmmoInMag = 0;

	return ReturnedAmount;
}

void AWeapon::Fire()
{
	if (AmmoInMag > 0 && bCanFire)
	{
		FVector BeamStart = MuzzleSceneComponent->GetComponentLocation();
		FVector BeamEnd = HitResultFromCharacter.TraceEnd;

		if (HitResultFromCharacter.bBlockingHit)
		{
			SpawnDecalAtHitLocation(HitResultFromCharacter);
			ApplyDamageIfDamagable(HitResultFromCharacter);

			BeamEnd = HitResultFromCharacter.ImpactPoint;
			SpawnImpactFX(HitResultFromCharacter.ImpactPoint, HitResultFromCharacter.ImpactNormal);
			
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *HitResultFromCharacter.GetActor()->GetName()));
		}

		SpawnTracer();

		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		if (AnimInstance && FireAnimMontage) {
			AnimInstance->Montage_Play(FireAnimMontage);
		}

		AmmoInMag -= 1;
		bCocked = false;
		CalculateCanFire();

		MakeNoise(1.f, GetWorld()->GetFirstPlayerController()->GetPawn(), GetActorLocation(), 0.f, TAG_GUNSHOT);
	}
}

void AWeapon::PlayFireAnimation()
{
	UAnimInstance* AnimInstance{ (SkeletalMeshComponent) ? SkeletalMeshComponent->GetAnimInstance() : nullptr };
	if (AnimInstance && FireAnimMontage)
	{
		AnimInstance->Montage_Play(FireAnimMontage);
	}
}

void AWeapon::SpawnDecalAtHitLocation_Implementation(FHitResult Param_Hit)
{
}

void AWeapon::InitiateFireSequence()
{
	
}

bool AWeapon::SetCocked(bool Param_bCocked)
{
	bCocked = Param_bCocked;

	CalculateCanFire();
	return bCocked;
}

void AWeapon::PlaySwapInAnimation()
{
	UAnimInstance* AnimInstance{ (SkeletalMeshComponent) ? SkeletalMeshComponent->GetAnimInstance() : nullptr };

	if (SwapInAnimMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(SwapInAnimMontage);
	}
}

void AWeapon::BeginPlaySetup()
{
	AmmoInMag = MagSize;

	// Set color data
	if (PaintMaterial)
	{
		switch (AmmoColor)
		{
		case EAmmoColor::EAC_Purple:
			LinearColor = PURPLE;
			break;
		case EAmmoColor::EAC_Yellow:
			LinearColor = YELLOW;
			break;
		case EAmmoColor::EAC_Green:
			LinearColor = GREEN;
			break;
		case EAmmoColor::EAC_Blue:
			LinearColor = BLUE;
			break;
		case EAmmoColor::EAC_Red:
			LinearColor = RED;
			break;
		}

		DynamicPaintMaterial = UMaterialInstanceDynamic::Create(PaintMaterial, this);
		DynamicPaintMaterial->SetVectorParameterValue(TEXT("Color"), LinearColor);
	}
}

