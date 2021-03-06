// Copyright Samuel Carriere 2020


#include "Grabber.h"

#define OUT

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

template<class T>
T* UGrabber::SafeGetComponent(FString name)
{
	T* Component = GetOwner()->FindComponentByClass<T>();
	FString ClassName(typeid(Component).name());
	if (!Component)
	{
		UE_LOG(LogTemp, Error, TEXT("No %s found on %s"), *ClassName, *GetOwner()->GetName());
	}
	return Component;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = SafeGetComponent<UPhysicsHandleComponent>(FString("Physics Handle Component"));
}

void UGrabber::SetupInputComponent()
{
	InputComponent = SafeGetComponent<UInputComponent>(FString("input Component"));
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::ToggleGrab);
	}
}

void UGrabber::ToggleGrab()
{
	if (!PhysicsHandle) { return; }

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
	else
	{
		FHitResult HitResult = GetFirstPhysicsBodyInReach();
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

		if (HitResult.GetActor())
		{
			PhysicsHandle->GrabComponentAtLocation(
				ComponentToGrab,
				NAME_None,
				//GetPlayersReach()
				HitResult.GetActor()->GetActorLocation()
			);
		}
	}
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldLocation(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach,
		FColor::Blue,
		false,
		0.f,
		0,
		1.f
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayersWorldLocation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}