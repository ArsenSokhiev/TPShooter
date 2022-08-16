// Third Player Shooter, All Rights Reserved

#include "Player/TPSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TPSCharacterMovementComponent.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ATPSBaseCharacter::ATPSBaseCharacter(const FObjectInitializer& ObjInit) 
	: ACharacter(ObjInit.SetDefaultSubobjectClass<UTPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
	// it.
	PrimaryActorTick.bCanEverTick = true;

	// ���� ��������� �����, ����� ������ ��������� �� ������ ���� � ������ ����� ����������
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	//SetupAttachment �� ����� (�� ����������� �� �����)
	HealthComponent = CreateDefaultSubobject<UTPSHealthComponent>("HealthComponent");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATPSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATPSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto Health = HealthComponent->GetHealth();
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

// Called to bind functionality to input
void ATPSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���������� ������ ����� (��� �������)
	//params: 1. mapping � project settings | 2. ��������� �� ������ ������� �������� ���������� | 3. ������ �� ����� :)
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ATPSBaseCharacter::TurnAround);

	//������� Jump ����������� � ������ Pawn
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSBaseCharacter::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATPSBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ATPSBaseCharacter::OnStopRunning);
}

void ATPSBaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	if (Amount == 0.0f) return;
	// params: 1. ������� ������������ ����������� � ������ ������ | 2. ����� ��������� � mapping | 3. 
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ATPSBaseCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}

void ATPSBaseCharacter::LookUp(float Amount)
{
	//param: �������� ����, ������� ����� ��������� � ��������
	AddControllerPitchInput(Amount);
}

void ATPSBaseCharacter::TurnAround(float Amount)
{
	//��������� �������� ����� (���������) ����� �������� �����������
	AddControllerYawInput(Amount);
}

bool ATPSBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !this->GetVelocity().IsZero();
}

float ATPSBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	//acos - ���������� ���� � ��������
	const auto Angle = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

	return FMath::RadiansToDegrees(Angle) * FMath::Sign(CrossProduct.Z);
}

void ATPSBaseCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ATPSBaseCharacter::OnStopRunning()
{
	WantsToRun = false;
}
