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
			SpawnedProjectile->SetDecalSize(DecalSize);
			SpawnedProjectile->SetDynamicMaterial(DynamicPaintMaterial);
			SpawnedProjectile->SetDamage(Damage);

			MakeNoise(1.f, GetWorld()->GetFirstPlayerController()->GetPawn(), GetActorLocation(), 0.f, TAG_GUNSHOT);

			AmmoInMag -= 1;
		}

		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		if (AnimInstance && FireAnimMontage) {
			AnimInstance->Montage_Play(FireAnimMontage);
		}

		MakeNoise(1.f, GetWorld()->GetFirstPlayerController()->GetPawn(), GetActorLocation(), 0.f, TAG_GUNSHOT);
	}
}

void AThumper::InitiateFireSequence()
{
	Fire();
}
