// Fill out your copyright notice in the Description page of Project Settings.


#include "VBS_ModuleActor.h"
#include "VBS_PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AVBS_ModuleActor::AVBS_ModuleActor() :
	_isBeingPlaced(false),
	_isShowingSnapMarkers(false),
	_isShowingConnectorMarkers(false),
	_CurrentCollisionState(EVBS_ModuleCollisionState::MCS_Detached),
	_moduleStaticMesh(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create the default scene object
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}

	// create the basic static mesh compoenent to hold the static mesh of this module...
	_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Module SMC"));
	_staticMeshComponent->SetupAttachment(RootComponent);

	// enable overlap collission...
	//_staticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_staticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	// Add default snap point
	_ModuleSnapPointOffsets.Add(FIntVector::ZeroValue);
}

// Called when the game starts or when spawned
void AVBS_ModuleActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Add code here WITH CAUTION!! it will be called as soon as the module is spawned but BEFORE the post-spawn initialization code...
	// To initialize stuff AFTER the post-spawn initialization use InitializeModule instead...
}

void AVBS_ModuleActor::InitializeModule()
{
	// if the static mesh was setup, select it into the SMC
	if (_staticMeshComponent)
		UpdateCurrentMesh();


	// now setup all the markers for snap/connector points... (we gonna use basic mesh from PC for this)
	AVBS_PlayerController* pc = GetWorld()->GetFirstPlayerController<AVBS_PlayerController>();
	if (pc)
	{
		UStaticMesh* snapMarkerMesh = pc->ModuleSnapMarkerMesh;
		UStaticMesh* connectorMarkerMesh = pc->ModuleConnectMarkerMesh;
		UMaterialInterface* markerMaterial = pc->ModuleMarkerMaterial;
		float gsize = pc->_snapGridSize;

		// snaps first...
		for (TCheckedPointerIterator<FIntVector, int32> iter = _ModuleSnapPointOffsets.begin(); iter != _ModuleSnapPointOffsets.end(); iter.operator++())
		{
			FVector relativePos = FVector((*iter)) * gsize;
			UStaticMeshComponent* marker = NewObject<UStaticMeshComponent>(this);
			marker->AttachToComponent(_staticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
			marker->SetStaticMesh(snapMarkerMesh);
			marker->SetMaterial(0, markerMaterial);
			marker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			marker->SetVisibility(false);
			marker->SetRelativeLocation(relativePos.GridSnap(gsize));
			marker->RegisterComponent();
			_snapPointMarkers.Add(marker);
		}

		// connector next
		for (TCheckedPointerIterator<FIntVector, int32> iter = _ModuleConnectorPointOffsets.begin(); iter != _ModuleConnectorPointOffsets.end(); iter.operator++())
		{
			FVector relativePos = FVector((*iter)) * gsize;
			UStaticMeshComponent* marker = NewObject<UStaticMeshComponent>(this);
			marker->AttachToComponent(_staticMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
			marker->SetStaticMesh(connectorMarkerMesh);
			marker->SetMaterial(0, markerMaterial);
			marker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			marker->SetVisibility(false);
			marker->SetRelativeLocation(relativePos.GridSnap(gsize));
			marker->RegisterComponent();
			_connectorPointMarkers.Add(marker);
		}

		ShowHideSnapMarkers(pc->ShouldShowSnapMarkers);
		ShowHideConnectorMarkers(pc->ShouldShowConnectionMarkers);
	}
}

void AVBS_ModuleActor::UpdateAllSnapMarkers()
{
	// snaps first...
	float gs = GetWorld()->GetFirstPlayerController<AVBS_PlayerController>()->_snapGridSize;
	int idx = 0;
	for (TCheckedPointerIterator<FIntVector, int32> iter = _ModuleSnapPointOffsets.begin(); iter != _ModuleSnapPointOffsets.end(); iter.operator++())
	{
		UStaticMeshComponent* marker = _snapPointMarkers[idx++];
		FVector relativePos = FVector((*iter)) * gs;
		marker->SetRelativeLocation(relativePos.GridSnap(gs));
	}

	// connector next
	idx = 0;
	for (TCheckedPointerIterator<FIntVector, int32> iter = _ModuleConnectorPointOffsets.begin(); iter != _ModuleConnectorPointOffsets.end(); iter.operator++())
	{
		UStaticMeshComponent* marker = _connectorPointMarkers[idx++];
		FVector relativePos = FVector((*iter)) * gs;
		marker->SetRelativeLocation(relativePos.GridSnap(gs));
	}
}

// Called every frame
void AVBS_ModuleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AVBS_ModuleActor::UpdateCurrentMesh()
{
	_staticMeshComponent->SetStaticMesh(_moduleStaticMesh);
}

void AVBS_ModuleActor::SetModuleRotation(EVBS_ModuleRotationPhase newRotationPhase_)
{
	FVector rotation = FVector::ZeroVector;
	switch (newRotationPhase_)
	{
	default:
	case EVBS_ModuleRotationPhase::MR_Phase0:
		rotation = FVector(0.0, 0.0, 0.0);
		break;
	case EVBS_ModuleRotationPhase::MR_Phase90:
		rotation = FVector(0.0, 90.0, 0.0);
		break;
	case EVBS_ModuleRotationPhase::MR_Phase180:
		rotation = FVector(0.0, 180.0, 0.0);
		break;
	case EVBS_ModuleRotationPhase::MR_Phase270:
		rotation = FVector(0.0, 270.0, 0.0);
		break;
	}

	SetActorLocationAndRotation(GetActorLocation(), rotation.Rotation());
}

void AVBS_ModuleActor::RotateModule(bool ccw_)
{
	FVector rotationFactor = (ccw_) ? FVector(0.0, 90.0, 0.0) : FVector(0.0, -90.0, 0.0);
	AddActorWorldRotation(rotationFactor.Rotation());
}

TArray<FVector> AVBS_ModuleActor::GetGridConnectorPoints()
{
	TArray<FVector> result;
	for (TCheckedPointerIterator<UStaticMeshComponent*, int32> iter = _connectorPointMarkers.begin();
		iter != _connectorPointMarkers.end();
		iter.operator++())
	{
		result.Add((*iter)->GetComponentLocation());
	}
	return result;
}

TArray<FVector> AVBS_ModuleActor::GetGridSnapPoints()
{
	TArray<FVector> result;
	for (TCheckedPointerIterator<UStaticMeshComponent*, int32> iter = _snapPointMarkers.begin();
		iter != _snapPointMarkers.end();
		iter.operator++())
	{
		result.Add((*iter)->GetComponentLocation());
	}
	return result;
}

bool AVBS_ModuleActor::IsOverlapingWithModule(AVBS_ModuleActor* otherActor_)
{
	if (otherActor_ == this)
		return false;

	TArray<FVector> mySnaps = GetGridSnapPoints();
	for (TCheckedPointerIterator<FVector, int32> iter1 = mySnaps.begin(); iter1 != mySnaps.end(); iter1.operator++())
	{
		TArray<FVector> otherSnaps = otherActor_->GetGridSnapPoints();
		for (TCheckedPointerIterator<FVector, int32> iter2 = otherSnaps.begin(); iter2 != otherSnaps.end(); iter2.operator++())
		{
			if ((*iter1).Equals(*iter2))
				return true;
		}
	}
	return false;
}

bool AVBS_ModuleActor::IsConnectingWithModule(AVBS_ModuleActor* otherActor_)
{
	if (otherActor_ == this)
		return false;

	bool imConnected = false, hesConnected = false;

	TArray<FVector> myConnectors = GetGridConnectorPoints();
	int idx = 0;
	for (TCheckedPointerIterator<FVector, int32> iter1 = myConnectors.begin(); iter1 != myConnectors.end(); iter1.operator++())
	{
		if ((*iter1).Equals(otherActor_->GetActorLocation(), 1.0))
		{
			imConnected = true;
			FString msg = FString::Printf(TEXT("IM CONNECTED! (ME)[%s] (HIM)[%s] on my snap id [%d]"), *GetName(), *(otherActor_->GetName()), idx);
			if (GEngine)
			{
				int uid = 313123;
				GEngine->AddOnScreenDebugMessage(uid, 5.0f, FColor::Red, msg);
			}
			break;
		}
		idx++;
	}

	TArray<FVector> otherConnectors = otherActor_->GetGridConnectorPoints();
	idx = 0;
	for (TCheckedPointerIterator<FVector, int32> iter2 = otherConnectors.begin(); iter2 != otherConnectors.end(); iter2.operator++())
	{
		if ((*iter2).Equals(GetActorLocation()))
		{
			hesConnected = true;
			FString msg = FString::Printf(TEXT("HES CONNECTED! (ME)[%s] (HIM)[%s] on HIS snap id [%d]"), *GetName(), *(otherActor_->GetName()), idx);
			if (GEngine)
			{
				int uid = 432434;
				GEngine->AddOnScreenDebugMessage(uid, 5.0f, FColor::Red, msg);
			}
			break;
		}
		idx++;
	}
	return imConnected && hesConnected;
}


void AVBS_ModuleActor::OverrideModuleMaterial(UMaterialInterface* overrideMaterial_)
{
	if (!_staticMeshComponent)
		return;

	_staticMeshComponent->SetMaterial(0, overrideMaterial_);
}

void AVBS_ModuleActor::ResetModuleMaterial()
{
	if (!_staticMeshComponent)
		return;
	_staticMeshComponent->SetMaterial(0, _defaultMaterial);
}

void AVBS_ModuleActor::ShowHideSnapMarkers(bool showMarkers_)
{
	for (TCheckedPointerIterator<UStaticMeshComponent*, int32> iter = _snapPointMarkers.begin();
		iter != _snapPointMarkers.end();
		iter.operator++())
	{
		(*iter)->SetVisibility(showMarkers_);
	}
	_isShowingSnapMarkers = showMarkers_;
}

void AVBS_ModuleActor::ShowHideConnectorMarkers(bool showMarkers_)
{
	for (TCheckedPointerIterator<UStaticMeshComponent*, int32> iter = _connectorPointMarkers.begin();
		iter != _connectorPointMarkers.end();
		iter.operator++())
	{
		(*iter)->SetVisibility(showMarkers_);
	}
	_isShowingConnectorMarkers = showMarkers_;
}