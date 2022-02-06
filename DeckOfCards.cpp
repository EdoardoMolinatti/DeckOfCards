// DeckOfCards.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// C includes
#include <cassert>
#include <cstdlib>
#include <ctime>
// C++ STL includes
#include <array>
#include <iomanip>
#include <iostream>

using namespace std;

// GLOBALS //
static const uint8_t sg_numCards = 52;


// Assume class used by many other people. Would be very difficult to change.
class IDeck {
    virtual void printFirst(uint8_t) = 0;
    virtual void shuffle() = 0;
    //virtual uint8_t size() = 0;
};

enum class Suit {
    Hearts,
    Spades,
    Diamonds,
    Clubs,
    SUIT_NUM    // Number of elements in the Suit enum
};

struct Card {
    Suit        _suit;
    uint32_t    _val;

    Card()
        : _suit(Suit::Hearts)
        , _val(1)
    { }

    void print() const {
        cout << setw(2) << _val << " ";
        switch (_suit)
        {
        case Suit::Hearts:
            cout << "Hearts" << endl;
            break;
        case Suit::Spades:
            cout << "Spades" << endl;
            break;
        case Suit::Diamonds:
            cout << "Diamonds" << endl;
            break;
        case Suit::Clubs:
            cout << "Clubs" << endl;
            break;
        default:
            cout << "N/A" << endl;
        }
    }
};

class Deck : public IDeck {
public:
    void printFirst(uint8_t num)
    {
        cout << "Printing the first " << static_cast<int>(num) << " cards in the deck:" << endl << flush;
        // Internally the std::array index is zero-based, hence (num - 1)
        for (int8_t idx = (num - 1); idx >= 0; --idx)
        {
            cout << "Index " << setw(2) << static_cast<int>(idx) << ": " << flush;
            m_cards[idx].print();
        }
        cout << endl;
    }

    void shuffle()
    {
        //for (const auto& card : m_cards)
        shuffle( static_cast<uint8_t>(m_cards.size()) );
    }

    void shuffle(uint32_t swaps)
    {
        cout << "Shuffling ";
        int totSwaps = 0;
        int deckSize = static_cast<int>(m_cards.size());
        for (uint32_t i = 0; i < swaps; ++i)
        {
            int firstIdx  = random(0, (deckSize - 1));
            int secondIdx = random(0, (deckSize - 1));
            if (firstIdx != secondIdx)
            {
                swap(m_cards[firstIdx], m_cards[secondIdx]);
                ++totSwaps;
                cout << ".";
            }
            else
            {
                //cout << "[" << firstIdx << "]";
                cout << "[" << firstIdx << "==" << secondIdx << "]";
            }
        }
        cout << " (" << totSwaps << " card swaps)" << endl << endl;
    }

    uint8_t size()
    {
        return static_cast<uint8_t>( m_cards.size() );
    }

    // Constructor
    Deck()
    {
        // Create a complete deck of cards
        const uint8_t suitsNum = static_cast<uint8_t>(Suit::SUIT_NUM);
        const uint8_t cardsPerSuit = static_cast<uint8_t>(m_cards.size()) / suitsNum;
        for (uint8_t i = 0; i < suitsNum; ++i)
        {
            int baseIdx = i * cardsPerSuit;

            for (uint8_t j = 0; j < cardsPerSuit; ++j)
            {
                m_cards[baseIdx + j]._suit = static_cast<Suit>(i);
                m_cards[baseIdx + j]._val  = static_cast<uint32_t>(j + 1);
            }
        }
    }

protected:
    array<Card, sg_numCards> m_cards;

private:
    // Random number in a range [min, max]
    int random(int min, int max)
    {
        static bool first = true;
        if (first)
        {
            // Random seed initialization (first time only)
            srand( static_cast<unsigned int>(time(NULL)) );
            first = false;
        }
        return min + rand() % ((max + 1) - min);
    }
};


bool testShuffle()
{
    bool testPassed = true;

    Deck deck;
    deck.shuffle();

    return testPassed;
}


bool testPrintFirst()
{
    bool testPassed = true;

    Deck deck;
    deck.printFirst(deck.size());

    return testPassed;
}


int main()
{
    Deck deck;

    //assert(testPrintFirst());
    //assert(testShuffle());

    deck.printFirst(deck.size());
    deck.shuffle();
    //deck.shuffle(512);
    deck.printFirst(deck.size());

    return 0;
}

// Tasks:
// 1. Roughly explain purpose of code, what should it do?
