// Fill out your copyright notice in the Description page of Project Settings.


#include "M4.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "DrawDebugHelpers.h"

AM4::AM4()
{
	WeaponName = TEXT("M4A1");
}

void AM4::InitiateReloadAnim()
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (AnimInstance && ReloadAnimMontage) {
		AnimInstance->Montage_Play(ReloadAnimMontage);

		bReloading = true;
		CalculateCanFire();
	}
}

void AM4::InitiateFireSequence()
{
	Fire();
}

bool AM4::SetCocked(bool Param_bCocked)
{
	bCocked = Param_bCocked;

	if (bTriggerPulled)
	{
		Super::Fire();
	}

	return bCocked;
}

int32 AM4::FinaliseReload(int32 AmmoReserve)
{
	int32 EmptyAmmo{ MagSize - AmmoInMag };

	if (EmptyAmmo > AmmoReserve)
	{
		AmmoInMag = AmmoReserve;
		AmmoReserve = 0;
	}
	else
	{
		AmmoInMag = MagSize;
		AmmoReserve -= EmptyAmmo;
	}

	bReloading = false;
	CalculateCanFire();

	return AmmoReserve;
}

void AM4::BeginPlay()
{
	Super::BeginPlay();
}



