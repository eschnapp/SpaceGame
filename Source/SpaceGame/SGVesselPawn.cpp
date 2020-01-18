// All copy rights reserved for RZ East Coast and papa r4z


#include "SGVesselPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/StaticMesh.h"

// Sets default values
ASGVesselPawn::ASGVesselPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create the default scene object
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}

	// create the basic static mesh compoenent to hold the static mesh of this module...
	VesselRootModuleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root Module"));
	VesselRootModuleComponent->SetupAttachment(RootComponent);

	// enable overlap collission...
	VesselRootModuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void ASGVesselPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASGVesselPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASGVesselPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASGVesselPawn::ConstructVesselProcedural(TArray<FSGVesselDescriptorItem> DescriptorArray)
{
	// first make sure that no module is specified at the same grid snap and that at leat 1 is root
	int bRootIndex = -1; 
	int idx = 0;
	bool bHasDups = false;
	TSet<FIntVector> locSet;

	for (FSGVesselDescriptorItem item : DescriptorArray)
	{
		if (locSet.Contains(item.OffsetFromRoot))
		{
			bHasDups = true;
			break;
		}

		if (item.OffsetFromRoot == FIntVector::ZeroValue)
		{
			bRootIndex = idx;
		}
		locSet.Add(item.OffsetFromRoot);
		idx++;
	}

	if (bHasDups || (bRootIndex <= -1))
	{
		// TODO: Some Error/log
		return;
	}

	// second loop to construct the vessel (should be alot slower here)
	idx = 0;
	for (FSGVesselDescriptorItem item : DescriptorArray)
	{
		if (idx == bRootIndex)
		{
			// this is the root module, set the correct mesh and ajust the rotation phase
			UStaticMesh* sm = GetStaticMeshForModule(item.ModuleType);
			if (!sm)
			{
				// TODO: Log error
				continue;
			}

			FVector rotationVector = FVector(item.RotationPhaseVector) * 90;
			VesselRootModuleComponent->SetStaticMesh(sm);
			VesselRootModuleComponent->AddWorldRotation(rotationVector.Rotation());
		}
		else
		{

		}
		idx++;
	}
}

UStaticMesh* ASGVesselPawn::GetStaticMeshForModule(EVBS_ModuleTypes ModuleType)
{
	return nullptr;
}
