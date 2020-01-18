// Fill out your copyright notice in the Description page of Project Settings.


#include "VBS_Character.h"
#include "VBS_Pawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
AVBS_Character::AVBS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
}

// Called when the game starts or when spawned
void AVBS_Character::BeginPlay()
{
	Super::BeginPlay();

	// Ensure that we are using the controller's rotation for camera travel....
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = true;


	// print a small debug msg to know we using this character
	if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AVBS_Character."));
	}
	
}

// Called every frame
void AVBS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVBS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind axis input for fwd/back and left/right controls (AWSD or whatever is currently set)
	PlayerInputComponent->BindAxis("MoveForward", this, &AVBS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVBS_Character::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AVBS_Character::MoveUp);
}

// Handle the movement of the chacater on the fwd/backwards axis by a given scale
void AVBS_Character::MoveForward(float Value)
{
	if (Value != 0)
	{
		// Find out which way is "forward" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		FVector NewLocation = GetActorLocation() + (Direction * Value);
		SetActorLocation(NewLocation); // Set new location causes a basic "teleport" effect... AddMovementInput would have added constant force moving in the direction..
		//AddMovementInput(Direction, Value); 
	}
	
}

// Handle the movement of the chacater on the left/right axis by a given scale
void AVBS_Character::MoveRight(float Value)
{
	if (Value != 0)
	{
		// Find out which way is "right" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		FVector NewLocation = GetActorLocation() + (Direction * Value);
		SetActorLocation(NewLocation);
		//AddMovementInput(Direction, Value);
	}
}

// Handle the movement of the chacater on the left/right axis by a given scale
void AVBS_Character::MoveUp(float Value)
{
	if (Value != 0)
	{
		// Find out which way is "right" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Z);
		FVector NewLocation = GetActorLocation() + (Direction * Value);
		SetActorLocation(NewLocation);
		//AddMovementInput(Direction, Value);
	}
}


