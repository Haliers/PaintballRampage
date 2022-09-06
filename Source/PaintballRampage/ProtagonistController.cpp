// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtagonistController.h"

#define PLAYER_TEAM_ID 0

AProtagonistController::AProtagonistController()
{
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(PLAYER_TEAM_ID);
}