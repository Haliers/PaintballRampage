// Fill out your copyright notice in the Description page of Project Settings.

#include "Glock.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "DrawDebugHelpers.h"

AGlock::AGlock()
{
	WeaponName = TEXT("Glock 19");
}

void AGlock::InitiateReloadAnim()
{
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if (AnimInstance && ReloadAnimMontage) {
		AnimInstance->Montage_Play(ReloadAnimMontage);

		bReloading = true;
		CalculateCanFire();
	}
}

void AGlock::InitiateFireSequence()
{
	Super::Fire();
}


bool AGlock::SetCocked(bool Param_bCocked)
{
	bCocked = Param_bCocked;

	return bCocked;
}

int32 AGlock::FinaliseReload(int32 AmmoReserve)
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

void AGlock::BeginPlay()
{
	Super::BeginPlay();
}
