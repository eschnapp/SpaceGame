// Fill out your copyright notice in the Description page of Project Settings.


#include "VEPlayerController.h"


AVEPlayerController::AVEPlayerController()
{
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
	EnableInput(this);
}

