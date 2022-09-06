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

void AIntervention::BeginPlay()
{
	Super::BeginPlay();
}
