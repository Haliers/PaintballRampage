// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Math/UnrealMathUtility.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Library.h"

AShotgun::AShotgun()
{
	WeaponName = TEXT("Shotgun");
}

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgun::Fire_Implementation()
{
	
}

void AShotgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgun::InitiateFireSequence()
{
	if (bCanFire && AmmoInMag > 0)
	{
		Fire();
		bCocked = false;

		CalculateCanFire();

		PlayFireAnimation();
	}
}

bool AShotgun::SetCocked(bool Param_bCocked)
{
	bCocked = Param_bCocked;

	CalculateCanFire();
	if (bCanFire)
	{
		ONSCREEN_DEBUG("true",1)
	}
	else
	{
		ONSCREEN_DEBUG("false", 1)
	}
	return bCocked;
}