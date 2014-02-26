#include "container.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>


namespace GUI
{

Container::Container()
: mChildren()
, mSelectedChild(-1)
{
}

Container::Container(sf::RenderWindow* window)
: mChildren()
, mSelectedChild(-1)
, mWindow(window)
{
}

void Container::pack(Component::Ptr component)
{
	mChildren.push_back(component);

	// ALW - If no Component is selected and Component is selectable then select it.
	if (!hasSelection() && component->isSelectable())
		select(mChildren.size() - 1);
}

bool Container::isSelectable() const
{
	return false;
}

void Container::handleEvent(const sf::Event& event)
{
	// If we have selected a child then give it events
	if (hasSelection() && mChildren[mSelectedChild]->isActive())
	{
		mChildren[mSelectedChild]->handleEvent(event);
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
		{
			selectPrevious();
		}
		else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
		{
			selectNext();
		}
		else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
		{
			if (hasSelection())
				mChildren[mSelectedChild]->activate();
		}
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		const sf::Vector2i cursorPosition = sf::Mouse::getPosition(*mWindow);
		int index = 0;
		for (const Component::Ptr& child : mChildren)
		{
			// ALW - TODO: The for loop does not end when a intersection is found
			const sf::Vector2f buttonPosition = child->getPosition();
			if (child->isSelectable())
			{
				if (isIntersect(cursorPosition, buttonPosition))
					select(index);
			}
			++index;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (hasSelection())
				mChildren[mSelectedChild]->activate();
		}
	}
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (const Component::Ptr& child : mChildren)
		target.draw(*child, states);
}

bool Container::hasSelection() const
{
	return mSelectedChild >= 0;
}

void Container::select(std::size_t index)
{
	if (mChildren[index]->isSelectable())
	{
		if (hasSelection())
			mChildren[mSelectedChild]->deselect();

		mChildren[index]->select();
		mSelectedChild = index;
	}
}

void Container::selectNext()
{
	if (!hasSelection())
		return;

	// Search next component that is selectable, wrap around if necessary
	int next = mSelectedChild;
	do
		next = (next + 1) % mChildren.size();
	while (!mChildren[next]->isSelectable());

	// Select that component
	select(next);
}

void Container::selectPrevious()
{
	if (!hasSelection())
		return;

	// Search previous component that is selectable, wrap around if necessary
	int prev = mSelectedChild;
	do
		prev = (prev + mChildren.size() - 1) % mChildren.size();
	while (!mChildren[prev]->isSelectable());

	// Select that component
	select(prev);
}

bool Container::isIntersect(const sf::Vector2i cursorPosition, const sf::Vector2f buttonPosition) const
{
	// ALW - TODO: Button width and height is hardcoded
	return (cursorPosition.x > buttonPosition.x) && (cursorPosition.x < buttonPosition.x + 200) 
					&& (cursorPosition.y > buttonPosition.y) && (cursorPosition.y < buttonPosition.y + 50);
}

}