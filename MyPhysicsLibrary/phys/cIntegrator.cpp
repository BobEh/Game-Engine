#include "cIntegrator.h"  // My header
#include <iostream>

namespace phys
{
	cIntegrator::cIntegrator()
	{
		
	}

	void cIntegrator::Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		float force = 10.0f;
		float mass = 1.0f;
		std::cout << "Please don't let me fall!!!!" << std::endl;

		pos = pos + vel * dt;
		vel = vel + (force / mass) * dt;
	}

	void cIntegrator::Euler(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{

		vel = vel + acc - (-appliedAcc) * dt;
		pos = pos + vel * dt;
		
	}

	// FOURTH-ORDER RUNGE-KUTTA
	//void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	//{
	//	// TODO: math!
	//	// For an excellent article on RK4 implementation:
	//	// https://gafferongames.com/post/integration_basics/
	//}

	//void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	//{
	//	// TODO: math!
	//	// For an excellent article on RK4 implementation:
	//	// https://gafferongames.com/post/integration_basics/
	//}
}