// Copyright Samuel Carriere 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGrabber();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	template<class T>
	T* SafeGetComponent(FString name);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();
	FHitResult GetFirstPhysicsBodyInReach() const;

	FVector GetPlayersReach() const;
	FVector GetPlayersWorldLocation() const;
};
