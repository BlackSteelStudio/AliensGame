// Fill out your copyright notice in the Description page of Project Settings.

#include "AliensProject.h"
#include "SoldierCharacter.h"


// Sets default values
ASoldierCharacter::ASoldierCharacter(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set FPS Mesh
	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("PawnMesh1P"));
	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->bOnlyOwnerSee = true;
	Mesh1P->bOwnerNoSee = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->bReceivesDecals = false;
	Mesh1P->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	Mesh1P->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	// Set TPS Mesh
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;

	// init les variables
	bIsTargeting = false;
	bIsRunning = false;

	runningSpeed = 1000.0f;
	walkSpeed = 600.0f;

	normalFOV = 90.0f;
	targetFOV = 60.0f;

}

// Called when the game starts or when spawned
void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// si isRunning est acitf change la vitesse du player
	const float speed = IsRunning() ? runningSpeed : walkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, speed, DeltaTime, 20.0f);

}

// Called to bind functionality to input
void ASoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Camera FPS Update
void ASoldierCharacter::OnCameraUpdate(const FVector& CameraLocation, const FRotator& CameraRotation)
{
	USkeletalMeshComponent* DefMesh1P = Cast<USkeletalMeshComponent>(GetClass()->GetDefaultSubobjectByName(TEXT("PawnMesh1P")));
	const FMatrix DefMeshLS = FRotationTranslationMatrix(DefMesh1P->RelativeRotation, DefMesh1P->RelativeLocation);
	const FMatrix LocalToWorld = ActorToWorld().ToMatrixWithScale();

	// Mesh rotating code expect uniform scale in LocalToWorld matrix

	const FRotator RotCameraPitch(CameraRotation.Pitch, 0.0f, 0.0f);
	const FRotator RotCameraYaw(0.0f, CameraRotation.Yaw, 0.0f);

	const FMatrix LeveledCameraLS = FRotationTranslationMatrix(RotCameraYaw, CameraLocation) * LocalToWorld.Inverse();
	const FMatrix PitchedCameraLS = FRotationMatrix(RotCameraPitch) * LeveledCameraLS;
	const FMatrix MeshRelativeToCamera = DefMeshLS * LeveledCameraLS.Inverse();
	const FMatrix PitchedMesh = MeshRelativeToCamera * PitchedCameraLS;

	Mesh1P->SetRelativeLocationAndRotation(PitchedMesh.GetOrigin(), PitchedMesh.Rotator());
}


//// Set Variables ////

// Set if Player Run or Walk
void ASoldierCharacter::SetRunningPlayer(bool isRun)
{
	SetRunningToServer(isRun);
}


//// Retournes Values ////

// Retourne la rotation de la camera utilisée pour l'animation de visée du personnage
FRotator ASoldierCharacter::GetCameraRotation() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

// Le joueur vise ?
bool ASoldierCharacter::IsTargeting() const
{
	return bIsTargeting;
}

// Le joueur cours
bool ASoldierCharacter::IsRunning() const
{
	return bIsRunning;
}

bool ASoldierCharacter::IsFirstPerson() const
{
	return Controller && Controller->IsLocalPlayerController();
}


//// Replication Network ////

// Send to Server Data
void ASoldierCharacter::SetRunningToServer_Implementation(bool isRunning)
{
	if (HasAuthority())
	{
		bIsRunning = isRunning;
		SetRunningToAll(isRunning);
	}
}
bool ASoldierCharacter::SetRunningToServer_Validate(bool isRunning)
{
	return true;
}

// Server Send to All data
void ASoldierCharacter::SetRunningToAll_Implementation(bool isRunning)
{
	bIsRunning = isRunning;
}
bool ASoldierCharacter::SetRunningToAll_Validate(bool isRunning)
{
	return true;
}

