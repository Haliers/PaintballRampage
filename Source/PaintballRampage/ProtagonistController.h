// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "ProtagonistController.generated.h"

/**
 * 
 */
UCLASS()
class PAINTBALLRAMPAGE_API AProtagonistController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AProtagonistController();

private:
	FGenericTeamId TeamId;
	FORCEINLINE FGenericTeamId GetGenericTeamId() const { return TeamId;  }
};
