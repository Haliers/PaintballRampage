// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoDrop.h"
#include "Math/UnrealMathUtility.h"
#include "Protagonist.h"
#include "Components/SphereComponent.h"
#include "Library.h"


// Sets default values
AAmmoDrop::AAmmoDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AAmmoDrop::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AAmmoDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoDrop::SetupAmmoData()
{
	switch (DropColor)
	{
	case EDropColor::EDC_Purple:
		AmmoMin = 3;
		AmmoMax = 9;
		break;
	case EDropColor::EDC_Yellow:
		AmmoMin = 0;
		AmmoMax = 2;
		break;
	case EDropColor::EDC_Green:
		AmmoMin = 0;
		AmmoMax = 1;
		break;
	case EDropColor::EDC_Blue:
		AmmoMin = 6;
		AmmoMax = 15;
		break;
	case EDropColor::EDC_Red:
		AmmoMin = 0;
		AmmoMax = 3;
		break;
	default:
		break;
	}

	Ammo = FMath::RandRange(AmmoMin, AmmoMax);
}

