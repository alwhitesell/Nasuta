#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>


struct Particle
{
	enum class Type
	{
		Propellant,
		Smoke,
		ParticleCount
	};

	sf::Vector2f	position;
	sf::Color		color;
	sf::Time		lifetime;
};

#endif
