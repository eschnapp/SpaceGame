// Fill out your copyright notice in the Description page of Project Settings.


#include "VBS_PopupMenuBase.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"


void UVBS_PopupMenuBase::HandleMenuButtonEvent(EVBS_MenuItems moduleType_)
{
	// Set Selected Menu ITem
	SelectedMenuItem = moduleType_;

	// Invalidate and move the panel to 0,0
	SetVisibility(ESlateVisibility::Hidden);
	SetPositionInViewport(FVector2D::ZeroVector);

	// execute the onclick delegate for other classes to react..
	OnMenuItemSelected.Broadcast(SelectedMenuItem);
}

void UVBS_PopupMenuBase::ShowMenu(FVector2D PositionInViewport )
{
	// Move to the requested location in viewport
	SetPositionInViewport(PositionInViewport);

	// show the menu
	SetVisibility(ESlateVisibility::Visible);
}