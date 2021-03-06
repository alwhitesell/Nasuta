#include "application.h"

#include "../Game/Resources/resourceIdentifiers.h"
#include "../Game/State/gameState.h"
#include "../Game/State/gameOverState.h"
#include "../Game/State/menuState.h"
#include "../Game/State/pauseState.h"
#include "../Game/State/settingsState.h"
#include "../Game/State/stateIdentifiers.h"
#include "../Game/State/titleState.h"

#include "Trambo/States/state.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <string>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(800, 600), "Nasuta", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mMusic()
, mSounds()
, mStateStack(trmb::State::Context(mWindow, mTextures, mFonts, mPlayer, mMusic, mSounds))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::ID::Main, "Data/Fonts/Sansation.ttf");

	mTextures.load(Textures::ID::TitleScreen, "Data/Textures/TitleScreen.png");
	mTextures.load(Textures::ID::Buttons,	  "Data/Textures/Buttons.png");

	mMusic.load(Music::ID::MenuTheme,    "Data/Music/MenuTheme.ogg");
	mMusic.load(Music::ID::MissionTheme, "Data/Music/MissionTheme.ogg");

	mSounds.load(SoundEffects::ID::AlliedGunfire, "Data/Sound/AlliedGunfire.wav");
	mSounds.load(SoundEffects::ID::EnemyGunfire,  "Data/Sound/EnemyGunfire.wav");
	mSounds.load(SoundEffects::ID::Explosion1,    "Data/Sound/Explosion1.wav");
	mSounds.load(SoundEffects::ID::Explosion2,    "Data/Sound/Explosion2.wav");
	mSounds.load(SoundEffects::ID::LaunchMissile, "Data/Sound/LaunchMissile.wav");
	mSounds.load(SoundEffects::ID::CollectPickup, "Data/Sound/CollectPickup.wav");
	mSounds.load(SoundEffects::ID::Button,        "Data/Sound/Button.wav");

	mStatisticsText.setFont(mFonts.get(Fonts::ID::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	registerStates();
	mStateStack.pushState(States::ID::Title);

	mMusic.setVolume(75.f);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::ID::Title);
	mStateStack.registerState<MenuState>(States::ID::Menu);
	mStateStack.registerState<GameState>(States::ID::Game);
	mStateStack.registerState<PauseState>(States::ID::Pause);
	mStateStack.registerState<SettingsState>(States::ID::Settings);
	mStateStack.registerState<GameOverState>(States::ID::GameOver);
}
