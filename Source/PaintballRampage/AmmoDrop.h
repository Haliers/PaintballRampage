// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoDrop.generated.h"

UENUM(BlueprintType)
enum class EDropColor : uint8
{
	EDC_Purple UMETA(DisplayName = "Purple"),
	EDC_Yellow UMETA(DisplayName = "Yellow"),
	EDC_Green UMETA(DisplayName = "Green"),
	EDC_Blue UMETA(DisplayName = "Blue"),
	EDC_Red UMETA(DisplayName = "Red")
};

UCLASS()
class PAINTBALLRAMPAGE_API AAmmoDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoDrop();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetupAmmoData();

private:
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 AmmoMin;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 AmmoMax;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EDropColor DropColor;
	
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UStaticMeshComponent* GetMesh() { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDropColor(EDropColor Param_DropColor) { DropColor = Param_DropColor; }

	FORCEINLINE EDropColor GetDropColor() { return DropColor; }

	FORCEINLINE int32 GetAmmo() { return Ammo; }

};
