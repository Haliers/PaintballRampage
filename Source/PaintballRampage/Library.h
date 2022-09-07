// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define ONSCREEN_DEBUG(str, t) GEngine->AddOnScreenDebugMessage(-1, t, FColor::Yellow, FString::Printf(TEXT(str)));
#define ONSCREEN_DEBUG_FLOAT(flt) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("%f"), flt));

// Niagara system color definitions
#define PURPLE FColor::FColor(230, 0, 156)
#define YELLOW FColor::FColor(243, 146, 0)
#define GREEN FColor::FColor(148, 193, 31)
#define BLUE FColor::FColor(29, 112, 184)
#define RED FColor::FColor(227, 6, 19)


// Noise Tags
#define TAG_GUNSHOT FName("Gunshot")

/**
 * 
 */
class PAINTBALLRAMPAGE_API Library
{
public:
	Library();
	~Library();
};
