// DeckOfCards.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// C includes
#include <cassert>
#include <cstdint>
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


// Assume class used by many other people. Would be very difficult to change.
class IDeck {
    virtual void printFirst(uint8_t) = 0;
    virtual void shuffle() = 0;
    //virtual uint8_t size() = 0;             // suggested method to add to the IDeck interface
};

enum class Suit : uint8_t {
    Hearts,
    Spades,
    Diamonds,
    Clubs,
    //WhiteHearts,
    //WhiteSpades,
    //WhiteDiamonds,
    //WhiteClubs,
    SUITS_NUM       // Number of elements in the Suit enum
};

// GLOBALS //
static const uint8_t sg_cardsPerSuit = 13; // [1..10] + J + Q + K
static const uint8_t sg_deckNumCards = sg_cardsPerSuit * static_cast<uint8_t>(Suit::SUITS_NUM);  // 52 - 65 - 78 - 91 - 104

struct Card {
    Suit        _suit;
    uint8_t     _val;

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
            cout << setw(2) << static_cast<unsigned>(_val);
            // N.B.: since _val is "uint8_t", and that type is defined as "unsigned char",
            // operator<<(ostream&, unsigned char) will be used, and that operator overload
            // would print the char value instead of the decimal value.
            // Casting to 'unsigned int' solves the problem ("unsigned" implies "unsigned int").
        }
        //----------------------------------------
        switch (_suit)
        {
        case Suit::Hearts:
            cout << "♥";    // Hearts
            break;
        case Suit::Spades:
            cout << "♠";    // Spades
            break;
        case Suit::Diamonds:
            cout << "♦";    // Diamonds
            break;
        case Suit::Clubs:
            cout << "♣";    // Clubs
            break;
        // https://en.wikipedia.org/wiki/Playing_cards_in_Unicode
        //case Suit::WhiteHearts:
        //    cout << "♡";    // WhiteHearts
        //    break;
        //case Suit::WhiteSpades:
        //    cout << "♤";    // WhiteSpades
        //    break;
        //case Suit::WhiteDiamonds:
        //    cout << "♢";    // WhiteDiamonds
        //    break;
        //case Suit::WhiteClubs:
        //    cout << "♧";    // WhiteClubs
        //    break;
        default:
            cout << "?";
        }
        cout << endl;
    }
};

class Deck : public IDeck {
public:
    // Method added to print all the cards in the Deck.
    void printAll()
    {
        const uint8_t totCards = static_cast<uint8_t>(m_cards.size());
        printFirst(totCards);
    }

    // Prints the first 'num' cards in the Deck.
    // They're in reverse order because a cards deck is supposed to be face down,
    // hence the cards extraction should starts from the deck's end.
    void printFirst(uint8_t num)
    {
        if (num < 1)
        {
            cout << "Nothing to print" << endl << flush;
            return;
        }

        const uint8_t deckSize = static_cast<uint8_t>(m_cards.size());
        if (1 == num)
        {
            cout << "Printing the first card in the deck:" << endl << flush;
        }
        else if (num > deckSize)
        {
            cout << "Cannot print the first " << static_cast<unsigned>(num) << " cards"
                " (the deck has only " << static_cast<unsigned>(deckSize) << " cards)." << endl;
            cout << "Printing all the cards in the deck (" << static_cast<unsigned>(deckSize) << "):" << endl << flush;
            num = deckSize;
        }
        else if (num == deckSize)
        {
            cout << "Printing all the cards in the deck (" << static_cast<unsigned>(num) << "):" << endl << flush;
        }
        else
        {
            cout << "Printing the first " << static_cast<unsigned>(num) << " cards in the deck:" << endl << flush;
        }

        const uint8_t numWidth = (num < 100) ? 2 : 3;
        // Starting from the back of the deck of cards (top), that's why we use reverse order
        // N.B.: the std::array index is zero-based, hence (deckSize - 1)
        for (int16_t idx = (deckSize - 1); idx >= (deckSize - num); --idx)
        {
            //cout << "Index " << setw(numWidth) << static_cast<int>(idx + 1) << ": " << flush;
            cout << "Card " << setw(numWidth) << static_cast<int>(idx + 1) << ": ";

            #pragma warning( disable : 28020 ) // Disable warning message C28020 ("The expression '...' is not true at this call")
            m_cards[idx].print();
            #pragma warning( default : 28020 ) // Re-enable it [https://learn.microsoft.com/en-us/cpp/code-quality/c28020]
        }
        cout << endl << flush;
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
                //cout << "[" << firstIdx << "↔" << secondIdx << "] ";
            }
            else
            {
                cout << "[" << firstIdx << "==" << secondIdx << "]";
                //cout << "[" << firstIdx << "]";
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
        const uint8_t suitsNum = static_cast<uint8_t>(Suit::SUITS_NUM);
        const uint8_t cardsPerSuit = static_cast<uint8_t>(m_cards.size()) / suitsNum;
        for (uint8_t i = 0; i < suitsNum; ++i)
        {
            uint8_t baseIdx = i * cardsPerSuit;

            for (uint8_t j = 0; j < cardsPerSuit; ++j)
            {
                m_cards[baseIdx + j]._suit = static_cast<Suit>(i);
                m_cards[baseIdx + j]._val  = (j + 1);
            }
        }
    }

protected:
    array<Card, sg_deckNumCards> m_cards;

private:
    // Returns a random number in a range [min, max]
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


// TESTS Start -----------------------------------------------------------------
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
    deck.printFirst(deck.size() / 2);

    return testPassed;
}
// TESTS End -------------------------------------------------------------------


int main()
{
#ifdef _WIN32
    // Set console code page to CodePage UTF8 so the console knows how to interpret string data
    SetConsoleOutputCP(CP_UTF8);
#   if _MSC_VER < 1930  // Visual Sudio 2022 compiler version starts from 19.30 (1930)
    // Enable buffering to prevent VS from chopping up UTF8 byte sequences
    setvbuf(stdout, nullptr, _IOFBF, 1024); // no need for this with VS 2022 and later
#   endif
#   ifndef NDEBUG
    std::string test1 = "Greek: αβγδ; German: Übergrößenträger;";
    std::string test2 = R"( ¯\_(ツ)_/¯)";
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Unicode UTF-8 tests.\n" << test1 << std::endl << test2 << std::endl;
    std::cout << "--------------------------------------------------" << std::endl << std::endl;
#   endif
#endif

    //assert(testPrintFirst());
    //assert(testShuffle());

    Deck deck;
    deck.printFirst(13);
    deck.printFirst(95);
    //deck.printAll(); //deck.printFirst(deck.size());
    deck.shuffle();
    //deck.shuffle(512);
    deck.printAll(); //deck.printFirst(deck.size());

    return 0;
}

// Tasks:
// 1. Roughly explain purpose of code, what should it do?
