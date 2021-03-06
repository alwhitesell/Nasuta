#include "titleState.h"
#include "stateIdentifiers.h"
#include "../Resources/resourceIdentifiers.h"

#include "Trambo/Utilities/utility.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>


TitleState::TitleState(trmb::StateStack& stack, trmb::State::Context context)
: trmb::State(stack, context)
, mText()
, mShowText(true)
, mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::TitleScreen));

	mText.setFont(context.fonts->get(Fonts::ID::Main));
	mText.setString("Press any key to start");
	trmb::centerOrigin(mText);
	mText.setPosition(sf::Vector2f(context.window->getSize() / 2u));
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);

	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	// If any key or button is pressed, trigger the next screen
	if (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonReleased) // ALW - TODO - Joystick UI
	{
		requestStackPop();
		requestStackPush(States::ID::Menu);
	}

	return true;
}
