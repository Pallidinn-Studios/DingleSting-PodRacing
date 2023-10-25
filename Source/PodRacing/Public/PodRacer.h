// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "Math/Vector2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "PodRacer.generated.h"

USTRUCT(BlueprintType)
struct FRaceGhost {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<FTransform> GhostTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<FVector2D> GhostYawThrottle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<FVector2D> GhostRollPitch;
};

UCLASS()
class PODRACING_API APodRacer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APodRacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Hover related vars
	void Hover();
	FVector Start;
	FVector End;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	
	FVector CalculatedVelocity;
	FRotator TiltRotation;
	 
	FTransform InitTransform;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Lap related functions / variables
	UFUNCTION(BlueprintCallable) void AddLapTime();
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float CurrentLapTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float GameStartTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<float> LapTimes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FRaceGhost SavedGhosts;

	
	//Blaster
	UFUNCTION(BlueprintCallable) void UseBlasters();
	UPROPERTY(BlueprintReadWrite, EditAnywhere) TArray<USceneComponent*> BlasterLocations;
	
	//Control functions
	UFUNCTION(BlueprintCallable) void RestartGame();
	UFUNCTION(BlueprintCallable) void TiltPod(FVector2D RollPitchInput);
	UFUNCTION(BlueprintCallable) void YawControl(FVector2D YawThrottleInput, FVector2D RollPitchInput);
	UFUNCTION(BlueprintCallable) void PodMovement(FVector2D YawThrottleInput, FVector2D RollPitchInput);
	
	//Visual
	UPROPERTY(BlueprintReadWrite, EditAnywhere) UStaticMeshComponent* PodRoot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* PodMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* LeftEngineParent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* LeftEngine;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USceneComponent* RightEngineParent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) USkeletalMeshComponent* RightEngine;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector EngineParentOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector EngineOffset = FVector(0, 650, 0);
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float PodRoll = 70;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float PodPitch = 5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FVector2D EngineTiltRange = FVector2D(10.0f, 70.0f);

	//Player settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere) float GlobalControlSensitivity = 10;

	//Flight settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float ForwardForce = 4000000.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float JoystickForce = 300000.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float RideHeight = 300;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float GroundDrag = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float GroundAngularDrag = 4;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float AirDrag = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flight settings") float AirAngularDrag = 1;

	//Misc
	UPROPERTY(BlueprintReadOnly) float PodSpeed;
	UPROPERTY(BlueprintReadOnly) bool IsGrounded;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

};
