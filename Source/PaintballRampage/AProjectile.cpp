// Fill out your copyright notice in the Description page of Project Settings.


#include "AProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AAProjectile::AAProjectile() :
	DamageRadius(100.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

