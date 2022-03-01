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

#ifdef _WIN32
#include <Windows.h>    // For UTF8 output support: SetConsoleOutputCP()
#endif

// Using directives //
using namespace std;

// GLOBALS //
static const uint8_t sg_numCards = 52;  // 52 - 65 - 78 - 91 - 104


// Assume class used by many other people. Would be very difficult to change.
class IDeck {
    virtual void printFirst(uint8_t) = 0;
    virtual void shuffle() = 0;
    //virtual uint8_t size() = 0;   // suggested method to add to the interface
};

enum class Suit {
    Hearts,
    Spades,
    Diamonds,
    Clubs,
    //WhiteHearts,
    //WhiteSpades,
    //WhiteDiamonds,
    //WhiteClubs,
    SUITS       // Number of elements in the Suit enum
};

struct Card {
    Suit        _suit;
    uint32_t    _val;

    Card()
        : _suit(Suit::Hearts)
        , _val(1)
    { }

    void print() const
    {
        switch (_val)
        {
        case 11:
            cout << setw(2) << "J";
            break;
        case 12:
            cout << setw(2) << "Q";
            break;
        case 13:
            cout << setw(2) << "K";
            break;
        case 1:
            cout << setw(2) << "A";
            break;
        default:
            cout << setw(2) << _val;
        }
        //----------------------------------------
        switch (_suit)
        {
        case Suit::Hearts:
            cout << "♥" << endl;    // Hearts
            break;
        case Suit::Spades:
            cout << "♠" << endl;    // Spades
            break;
        case Suit::Diamonds:
            cout << "♦" << endl;    // Diamonds
            break;
        case Suit::Clubs:
            cout << "♣" << endl;    // Clubs
            break;
        // https://en.wikipedia.org/wiki/Playing_cards_in_Unicode
        //case Suit::WhiteHearts:
        //    cout << "♡" << endl;    // WhiteHearts
        //    break;
        //case Suit::WhiteSpades:
        //    cout << "♤" << endl;    // WhiteSpades
        //    break;
        //case Suit::WhiteDiamonds:
        //    cout << "♢" << endl;    // WhiteDiamonds
        //    break;
        //case Suit::WhiteClubs:
        //    cout << "♧" << endl;    // WhiteClubs
        //    break;
        default:
            cout << "?" << endl;
        }
    }
};

class Deck : public IDeck {
public:
    // Method added to print all the cards in the Deck (in straight order)
    void printAll()
    {
        const int16_t totCards = static_cast<int16_t>(m_cards.size());
        const int16_t numWidth = (totCards < 100) ? 2 : 3;
        for (int16_t idx = 0; idx < totCards; ++idx)
        {
            cout << "Index " << setw(numWidth) << idx << ": " << flush;
            m_cards[idx].print();
        }
        cout << endl;
    }

    void printFirst(uint8_t num)
    {
        cout << "Printing the first " << static_cast<int>(num) << " cards in the deck:" << endl << flush;
        // Internally the std::array index is zero-based, hence (num - 1)
        for (int16_t idx = (num - 1); idx >= 0; --idx)
        {
            cout << "Index " << setw(2) << static_cast<int>(idx) << ": " << flush;
            m_cards[idx].print();
        }
        cout << endl;
    }

    void shuffle()
    {
        shuffle( static_cast<uint32_t>(m_cards.size()) );
    }

    void shuffle(uint32_t swaps)
    {
        cout << "Shuffling ";
        uint32_t totSwaps = 0;
        uint32_t deckSize = static_cast<uint32_t>(m_cards.size());
        //for (const auto& card : m_cards)
        for (uint32_t i = 0; i < swaps; ++i)
        {
            uint32_t firstIdx  = random(0, (deckSize - 1));
            uint32_t secondIdx = random(0, (deckSize - 1));
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
        cout << endl << "(" << totSwaps << " card swaps)" << endl << endl;
    }

    uint8_t size()
    {
        return static_cast<uint8_t>( m_cards.size() );
    }

    // Constructor
    Deck()
    {
        // Create a complete deck of cards
        const uint8_t suitsNum = static_cast<uint8_t>(Suit::SUITS);
        const uint8_t cardsPerSuit = static_cast<uint8_t>(m_cards.size()) / suitsNum;
        for (uint8_t i = 0; i < suitsNum; ++i)
        {
            uint8_t baseIdx = i * cardsPerSuit;

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
#ifdef _WIN32
    // Set console code page to CodePage UTF8 so console known how to interpret string data
    SetConsoleOutputCP(CP_UTF8);
    // Enable buffering to prevent VS from chopping up UTF8 byte sequences
    //setvbuf(stdout, nullptr, _IOFBF, 1024); // [no need for this anymore]

    //std::string test = "Greek: αβγδ; German: Übergrößenträger;";// R"( ¯\_(ツ)_/¯)";
    //std::cout << test << std::endl;
#endif

    //assert(testPrintFirst());
    //assert(testShuffle());

    Deck deck;
    deck.printAll(); //deck.printFirst(deck.size());
    deck.shuffle();
    //deck.shuffle(512);
    deck.printAll(); //deck.printFirst(deck.size());

    return 0;
}

// Tasks:
// 1. Roughly explain purpose of code, what should it do?
