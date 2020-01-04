// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VEPlayerController.generated.h"

/**
 * PlayerController specialized for the Vessel Editor level 
 */
UCLASS()
class SPACEGAME_API AVEPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AVEPlayerController();
};
