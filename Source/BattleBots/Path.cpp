// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleBots.h"
#include "Path.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "AI/Navigation/NavigationPath.h"

// Sets default values for this component's properties
UPath::UPath()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    MoveToSpline = CreateDefaultSubobject<USplineComponent>(FName("MoveToSpline"));
    MoveToSpline->ClearSplinePoints();
}


// Called when the game starts
void UPath::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPath::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UPath::DrawNavPath(FVector PathEnd)
{
    UNavigationPath* tpath;
    UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(GetWorld());
    
    AActor* Actor = GetOwner();
    FVector PathStart;
    if (MoveToSpline->GetNumberOfSplinePoints() == 0)
    {
        PathStart = Actor->GetActorLocation();
    }
    else
    {
        PathStart = MoveToSpline->GetLocationAtSplinePoint((MoveToSpline->GetNumberOfSplinePoints() - 1), ESplineCoordinateSpace::Local);
    }
    
    // get the points to move to location
    tpath = NavSys->FindPathToLocationSynchronously(GetWorld(), PathStart, PathEnd, Actor);
    
    /*
    // iterate each spline mesh comp array and delete existing spline meshes
    for (auto& thisMesh : SplineMeshPtrArray)
    {
        thisMesh->DestroyComponent();
    }
    SplineMeshPtrArray.Empty();
    
    // iterate each static mesh comp array and delete existing meshes
    for (auto& thisMesh : StaticMeshPtrArray)
    {
        thisMesh->DestroyComponent();
        //UE_LOG(LogTemp, Warning, TEXT("Deleted Static Mesh."))
    }
    StaticMeshPtrArray.Empty();

    MoveToSpline->ClearSplinePoints();
    */
    
    if (tpath != NULL)
    {
        // iterate through each point in moveto destination and add points to spline
        auto SplineStartIndex = MoveToSpline->GetNumberOfSplinePoints();
        // TODO these loops can probably be combined into one
        for (auto i = 0; i < tpath->PathPoints.Num(); i++)
        {
            MoveToSpline->AddSplineLocalPoint(tpath->PathPoints[i]); // TODO protect pointer
        }
        
        // iterate through spline points and add spline mesh(es)
        //   stop at 1 fewer than number of points because last point won't need spline mesh created
        for (auto i = SplineStartIndex; i < (MoveToSpline->GetNumberOfSplinePoints() - 1); i++)
        {
            // create a spline mesh component
            USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this);
            if (SplineMesh)
            {
                SplineMeshPtrArray.Add(SplineMesh);

                SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
                
                //SplineMesh->bCreatedByConstructionScript = true;
                SplineMesh->SetMobility(EComponentMobility::Movable);
                SplineMesh->AttachToComponent(MoveToSpline, FAttachmentTransformRules::KeepRelativeTransform);
                
                SplineMesh->SetForwardAxis(ESplineMeshAxis::Z, true);
                
                //Set the color!
                //UMaterialInstanceDynamic* dynamicMat = UMaterialInstanceDynamic::Create(mSplineMeshMaterial, NULL);
                //dynamicMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(mSegments[i].mColor));
                
                //SplineMesh->bCastDynamicShadow = false;
                SplineMesh->SetStaticMesh(MoveToLineMesh); // TODO protect pointer
                SplineMesh->SetMaterial(0, LineMeshMaterial); // TODO protect pointer
                
                //Width of the mesh
                //SplineMesh->SetStartScale(FVector2D(50, 50));
                //SplineMesh->SetEndScale(FVector2D(50, 50));
                
                // set start/end location and tangent to this point and the next point respectively
                FVector pointLocationStart, pointTangentStart, pointLocationEnd, pointTangentEnd;
                MoveToSpline->GetLocalLocationAndTangentAtSplinePoint(i, pointLocationStart, pointTangentStart);
                MoveToSpline->GetLocalLocationAndTangentAtSplinePoint(i + 1, pointLocationEnd, pointTangentEnd);

                SplineMesh->SetStartAndEnd(pointLocationStart + FVector(0, 0, MoveToLineHeight), pointTangentStart, pointLocationEnd + FVector(0, 0, MoveToLineHeight), pointTangentEnd);
            }
        }
        
        auto SplineEndIndex = MoveToSpline->GetNumberOfSplinePoints() - 1;
        
        // Change last point to linear
        if (MoveToSpline)
        {
            MoveToSpline->SetSplinePointType(SplineEndIndex, ESplinePointType::Linear, true);
        }
        
        // add disc mesh to end of spline
        auto DiscLocation = MoveToSpline->GetLocationAtSplinePoint(SplineEndIndex, ESplineCoordinateSpace::Local);
        UStaticMeshComponent* DiscMesh = NewObject<UStaticMeshComponent>(this);
        if (DiscMesh)
        {
            StaticMeshPtrArray.Add(DiscMesh);
            DiscMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
            DiscMesh->SetMobility(EComponentMobility::Movable);
            DiscMesh->AttachToComponent(MoveToSpline, FAttachmentTransformRules::KeepRelativeTransform);
            DiscMesh->SetStaticMesh(PathPointDisc);
            DiscMesh->SetMaterial(0, PathPointMaterial);
            DiscMesh->SetWorldLocationAndRotation(DiscLocation + FVector(0, 0, MoveToLineHeight), FRotator(0.f));
        }
        
        Actor->RegisterAllComponents();
    }
}

float UPath::GetPathLength()
{
    return MoveToSpline->GetSplineLength();
}
