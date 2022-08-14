// Third Player Shooter, All Rights Reserved


#include "Components/TPSCharacterMovementComponent.h"
#include "Player/TPSBaseCharacter.h"

float UTPSCharacterMovementComponent::GetMaxSpeed() const
{
	//������ MaxSpeed �� �������� ������
	const float MaxSpeed = UCharacterMovementComponent::GetMaxSpeed();

	//���������� ��������� �� ����(�����������)
	const ATPSBaseCharacter* Player = Cast<ATPSBaseCharacter>(this->GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}