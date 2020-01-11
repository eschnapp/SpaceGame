// Fill out your copyright notice in the Description page of Project Settings.


#include "VEGameModeBase.h"
#include "VEPlayerController.h"


AVEGameModeBase::AVEGameModeBase()
{
	// Set the default player controller to be the Vessel Editor's default controller
	PlayerControllerClass = AVEPlayerController::StaticClass();
}

