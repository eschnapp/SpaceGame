// Fill out your copyright notice in the Description page of Project Settings.


#include "VBS_GameModeBase.h"
#include "VBS_PlayerController.h"
#include "VBS_Pawn.h"
#include "VBS_Character.h"

// Default Ctor
AVBS_GameModeBase::AVBS_GameModeBase()
{
	// set the default player controller class to be the VBS player controller.
	PlayerControllerClass = AVBS_PlayerController::StaticClass();

	// set the default pawn class to be the VBS character
	DefaultPawnClass = AVBS_Character::StaticClass();
	
}