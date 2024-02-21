
#include <iostream>

// Add a card to the list
void CardList::addCard(Card* card) {
    if (card != nullptr) {
        m_cards.push_back(card);
    } else {
        std::cerr << "The card you're trying to add to CardList does not exist.\n";
    }
}

// Remove a card from the list; argument is a card pointer
void CardList::removeCard(Card* card) {
    /* Find the card in the list */
    int index{ 0 };
    while (index < m_cards.size() && m_cards[index] != card) {
        ++index;
    }

    removeCard(index);
}

// Remove a card from the list; argument is an index
void CardList::removeCard(int index) {
    /* Remove the card if it exists */
    if (index < m_cards.size()) {
        m_cards.erase(m_cards.begin() + index);
    } else {
        std::cerr << "The card you're trying to remove from CardList could not be found.\n";
    }
}

// Get a card from the list at a specified index
Card* CardList::getCard(int index) {
    if (index < m_cards.size()) {
        return m_cards[index];
    } else {
        std::cerr << "The card you're trying to reach is out-of-bounds.\n";
        return nullptr;
    }
}

// Get a card from the list at a specified index
Card* CardList::getCard(int index) const {
    if (index < m_cards.size()) {
        return m_cards[index];
    } else {
        std::cerr << "The card you're trying to reach is out-of-bounds.\n";
        return nullptr;
    }
}
