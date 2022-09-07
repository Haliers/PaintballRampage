// Fill out your copyright notice in the Description page of Project Settings.


#include "Thumper.h"
#include "AProjectile.h"
#include "Library.h"

void AThumper::Fire()
{
	if (AmmoInMag > 0 && bCanFire)
	{
		if (ProjectileClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AAProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AAProjectile>(ProjectileClass, MuzzleSceneComponent->GetComponentTransform(), SpawnParams);
			SpawnedProjectile->SetDamageRadius(DamageRadius);

			AmmoInMag -= 1;
		}
	}
}

void AThumper::InitiateFireSequence()
{
	Fire();
}
