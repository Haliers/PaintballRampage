// Fill out your copyright notice in the Description page of Project Settings.


#include "OfficerDunkin.h"
#include "AProjectile.h"
#include "Library.h"

void AOfficerDunkin::ThrowDonut()
{
	ONSCREEN_DEBUG("Donut throw", 1)

	if (DonutClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AAProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AAProjectile>(DonutClass, ThrowSceneComponent->GetComponentTransform(), SpawnParams);
		SpawnedProjectile->SetDamageRadius(DamageRadius);
		SpawnedProjectile->SetDecalSize(DecalSize);
		SpawnedProjectile->SetDynamicMaterial(DynamicPaintMaterial);
		SpawnedProjectile->SetDamage(Damage);	
	}
}
