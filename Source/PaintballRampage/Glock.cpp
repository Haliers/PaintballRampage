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

void AGlock::BeginPlay()
{
	Super::BeginPlay();
}
