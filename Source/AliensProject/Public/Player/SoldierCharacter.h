// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "SoldierCharacter.generated.h"

UCLASS()
class ALIENSPROJECT_API ASoldierCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldierCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* Add camera pitch to first person mesh.
	*
	*	@param	CameraLocation	Location of the Camera.
	*	@param	CameraRotation	Rotation of the Camera.
	*/
	void OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation);

	//// BluePrint Fonction ////

	/** Set Player to run or walk */
	UFUNCTION(BlueprintCallable, Category = "Perso")
		void SetRunningPlayer(bool isRun);

	/** Get targeting state */
	UFUNCTION(BlueprintCallable, Category = "Perso")
		bool IsTargeting() const;

	/** is FPS ? */
	UFUNCTION(BlueprintCallable, Category = "Perso")
		bool IsFirstPerson() const;

	/** Get Camera Rotation */
	UFUNCTION(BlueprintCallable, Category = "Perso")
		FRotator GetCameraRotation() const;

	/* get if Player run */
	UFUNCTION(BlueprintCallable, Category = "Perso")
		bool IsRunning() const;

	//// Network ////

	UFUNCTION(Reliable, Server, WithValidation)
		void SetRunningToServer(bool isRunning);
	void SetRunningToServer_Implementation(bool isRunning);
	bool SetRunningToServer_Validate(bool isRunning);

	UFUNCTION(Reliable, NetMulticast, WithValidation)
		void SetRunningToAll(bool isRunning);
	void SetRunningToAll_Implementation(bool isRunning);
	bool SetRunningToAll_Validate(bool isRunning);

	//// Variables ////

	/** Active la visée */
	UPROPERTY(BlueprintReadWrite, Category = "Perso")
		bool bIsTargeting;

	UPROPERTY(BlueprintReadWrite, Category = "Perso")
		bool bIsRunning;

	/*  Set run speed */
	UPROPERTY(BlueprintReadWrite, Category = "Perso")
		float runningSpeed;

	/*  Set walk speed */
	UPROPERTY(BlueprintReadWrite, Category = "Perso")
		float walkSpeed;

	/** Normal FOV */
	UPROPERTY(BlueprintReadWrite, Category = "Perso")
		float normalFOV;

	/** Target FOV */
	UPROPERTY(BlueprintReadWrite, Category = "Perso")
		float targetFOV;

private:

	/** Mesh FPS */
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
		USkeletalMeshComponent* Mesh1P;

	/** Mesh FPS */
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		USkeletalMeshComponent* Weapon;


};
