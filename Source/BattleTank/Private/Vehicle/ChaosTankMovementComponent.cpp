// Fill out your copyright notice in the Description page of Project Settings.

#include "Vehicle/ChaosTankMovementComponent.h"
#include "VehicleUtility.h"


using namespace Chaos;

void UChaosTankVehicleSimulation::ProcessSteering(const FControlInputs& ControlInputs)
{
	//UChaosWheeledVehicleSimulation::ProcessSteering(ControlInputs);

	auto& PSteering = PVehicle->GetSteering();

	for (int WheelIdx = 0; WheelIdx < PVehicle->Wheels.Num(); WheelIdx++)
	{
		auto& PWheel = PVehicle->Wheels[WheelIdx]; // Physics Wheel
		FHitResult& HitResult = WheelState.TraceResult[WheelIdx];

		if (PWheel.SteeringEnabled)
		{
			float SpeedScale = 1.0f;

			// allow full counter steering when steering into a power slide
			//if (ControlInputs.SteeringInput * VehicleState.VehicleLocalVelocity.Y > 0.0f)
			{
				SpeedScale = PVehicle->GetSteering().GetSteeringFromVelocity(CmSToMPH(VehicleState.ForwardSpeed));
			}

			float SteeringInputFactor = (PWheel.Setup().AxleType == FSimpleWheelConfig::EAxleType::Rear) ? -1.f : 1.f;
			
			
			float SteeringAngle = ControlInputs.SteeringInput * SpeedScale * SteeringInputFactor;

			static const auto CVarSteeringOverride = IConsoleManager::Get().FindConsoleVariable(TEXT("p.Vehicle.SteeringOverride"));
			float SteeringOverride = CVarSteeringOverride->GetFloat();

			if (FMath::Abs(SteeringOverride) > 0.01f)
			{
				SteeringAngle = PWheel.MaxSteeringAngle * SteeringOverride;
			}
			else
			{
				float WheelSide = PVehicle->GetSuspension(WheelIdx).GetLocalRestingPosition().Y;
				SteeringAngle = PSteering.GetSteeringAngle(SteeringAngle, PWheel.MaxSteeringAngle, WheelSide);
			}



			PWheel.SetSteeringAngle(SteeringAngle);
		}
		else
		{
			PWheel.SetSteeringAngle(0.0f);
		}
	}

}
