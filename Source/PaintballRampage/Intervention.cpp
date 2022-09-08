// Fill out your copyright notice in the Description page of Project Settings.


#include "Intervention.h"
#include "Library.h"

AIntervention::AIntervention()
{
	WeaponName = TEXT("Intervention");
}

void AIntervention::InitiateFireSequence()
{
	Fire();
}

void AIntervention::Fire()
{
	if (AmmoInMag > 0 && bCanFire)
	{
		FVector BeamStart = MuzzleSceneComponent->GetComponentLocation();
		FVector BeamEnd = HitResultFromCharacter.TraceEnd;

		TArray<FHitResult> InterventionHits;
		GetWorld()->LineTraceMultiByChannel(InterventionHits, BeamStart,BeamEnd,ECollisionChannel::ECC_Visibility);

		if (InterventionHits.Num() > 0)
		{
			SpawnDecalAtHitLocation(InterventionHits.Last().ImpactPoint, InterventionHits.Last().ImpactNormal);
			ApplyDamageIfDamagable(InterventionHits);

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

void AIntervention::BeginPlay()
{
	Super::BeginPlay();
}
