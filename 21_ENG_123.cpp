#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;

// class for card shapes(spades,clubs,hearts,diamonds)
enum shapesOfCards
{
    spades,clubs,hearts,diamonds
};

// class for card values
enum valuesOfCards
{
    two = 2,three,four,five,six,seven,eight,nine,ten,jack,queen,king,ace
};

// class for initialize the combinations
enum HandCombination
{
    HighCard = 100,OnePair = 200,TwoPair = 300,ThreeOfAKind = 400,Straight = 500,Flush = 600,FullHouse = 700,FourOfAKind = 800,StraightFlush = 900
};

class Card
{
public:
    valuesOfCards value;
    shapesOfCards shape;
};

class Deck
{
public:
    vector<Card> cards;

    Deck()
    {
        Card card;
        for (int i = two; i <= ace; i++)
        {
            card.value = static_cast<valuesOfCards>(i);
            for (int j = spades; j <= diamonds; j++)
            {
                card.shape = static_cast<shapesOfCards>(j);
                cards.push_back(card);
            }
        }
        cardShuffling();
    }

    void cardShuffling()
    {
        random_device randomDevice;
        mt19937 randomGenerator(randomDevice());
        shuffle(cards.begin(), cards.end(), randomGenerator);
    }
};

class Player
{
public:
    Deck hand;
    vector<Card> cards;
    int totalScore;
    float probabilityOfWinning;
    bool playingCheck;

    Player() : totalScore(0), probabilityOfWinning(0), playingCheck(true) {}

    /* bool shouldWithdraw()
    {
        return false;
    } */

    int calculateHandScore()
{
    int score = 0;

    map<valuesOfCards, int> cardValueCounts;
    for (const auto &card : cards)
    {
        cardValueCounts[card.value]++;
    }

    int maximumCount = 0;
    for (const auto &count : cardValueCounts)
    {
        maximumCount = max(maximumCount, count.second);
    }

    map<shapesOfCards, int> cardShapeCounts;
    for (const auto &card : cards)
    {
        cardShapeCounts[card.shape]++;
    }

    bool checkFlush = false;
    for (const auto &count : cardShapeCounts)
    {
        if (count.second == 5)
        {
            checkFlush = true;
            break;
        }
    }

    vector<valuesOfCards> values;
    for (const auto &card : cards)
    {
        values.push_back(card.value);
    }
    sort(values.begin(), values.end());

    bool isStraight = false;
    for (int i = 0; i < values.size() - 4; i++)
    {
        if (values[i + 4] - values[i] == 4)
        {
            isStraight = true;
            break;
        }
    }

    valuesOfCards highValue = values.back();

    switch (maximumCount)
    {
    case 4:
        score = FourOfAKind + highValue;
        break;
    case 3:
        if (cardValueCounts.size() == 2)
        {
            score = FullHouse + highValue;
        }
        else
        {
            score = ThreeOfAKind + highValue;
        }
        break;
    default:
        if (isStraight && checkFlush)
        {
            score = StraightFlush + highValue;
        }
        else if (checkFlush)
        {
            score = Flush + highValue;
        }
        else if (isStraight)
        {
            score = Straight + highValue;
        }
        else if (cardValueCounts.size() == 3)
        {
            score = TwoPair + highValue;
        }
        else if (cardValueCounts.size() == 4)
        {
            score = OnePair + highValue;
        }
        else
        {
            score = HighCard + highValue;
        }
        break;
    }

    return score;
}

};

void dealingCards(vector<Player> &players, Deck shuffledCardDeck)
{
    // Removing the first two cards
    shuffledCardDeck.cards.erase(shuffledCardDeck.cards.begin());
    shuffledCardDeck.cards.erase(shuffledCardDeck.cards.begin());

    for (int i = 0; i < players.size(); i++)
    {
        players[i].cards.clear();
        for (int j = 0; j < 5; j++)
        {
            if (!players[i].playingCheck)
                continue;

            players[i].cards.push_back(shuffledCardDeck.cards[0]);
            shuffledCardDeck.cards.erase(shuffledCardDeck.cards.begin());
        }
    }
}

void ScoreUpdate(vector<Player> &players)
{
    int maximumScore = 0;
    int winner = 0;

    // Finding the player with the highest score
    for (int i = 1; i < players.size(); i++)
    {
        if (players[i].playingCheck)
        {
            int score = players[i].calculateHandScore();
            if (score > maximumScore)
            {
                maximumScore = score;
                winner = i;
            }
        }
    }

    // Update scores
    for (int i = 1; i < players.size(); i++)
    {
        if (players[i].playingCheck)
        {
            if (i == winner)
            {
                players[i].totalScore += 10;
            }
            else
            {
                players[i].totalScore -= 1;
            }
        }
    }
}



void updateProbability(vector<Player> &players)
{
    int totalCurrentScore = 0;
    for (int i = 1; i < players.size(); i++)
    {
        if (players[i].playingCheck)
            totalCurrentScore += players[i].calculateHandScore();
    }

    for (int i = 1; i < players.size(); i++)
        if (players[i].playingCheck)
            players[i].probabilityOfWinning = (float(players[i].calculateHandScore()) / float(totalCurrentScore)) * 100.0;
        else
            players[i].probabilityOfWinning = 0.0;
}

string playerCards(Player player)
{
    string details = "";
    for (int i = 0; i < player.cards.size(); i++)
    {
        // Value
        switch (player.cards[i].value)
        {
        case two:
            details += "two ";
            break;
        case three:
            details += "three ";
            break;
        case four:
            details += "four ";
            break;
        case five:
            details += "five ";
            break;
        case six:
            details += "six ";
            break;
        case seven:
            details += "seven ";
            break;
        case eight:
            details += "eight ";
            break;
        case nine:
            details += "nine ";
            break;
        case ten:
            details += "ten ";
            break;
        case jack:
            details += "jack ";
            break;
        case queen:
            details += "queen ";
            break;
        case king:
            details += "king ";
            break;
        case ace:
            details += "ace ";

        default:
            break;
        }

        details += "of ";

        // Shape
        switch (player.cards[i].shape)
        {
        case spades:
            details += "spades";
            break;
        case clubs:
            details += "clubs";
            break;
        case hearts:
            details += "hearts";
            break;
        case diamonds:
            details += "diamonds";
            break;

        default:
            break;
        }

        details += " | ";
        
    }

    return details;
}
void PokerGame(string details)
{
    fstream file;
    file.open("PokerGame.txt", ios::app);

    file << details << endl;
    file.close();
}

void simulatePokerGame()
{
    int noOfPlayers = 0;
    int noOfRounds = 0;
    int winner = 0;

    cout << "Number of players playing the Poker Game: ";
    cin >> noOfPlayers;
    noOfPlayers++; 

    cout << "Number of rounds playing: ";
    cin >> noOfRounds;

    Deck shuffledCardDeck;

    vector<Player> players;
    players.resize(noOfPlayers);

    for (int i = 0; i < noOfRounds; i++)
    {
        cout << endl;
        cout << "Round " << i + 1 << endl
             << endl;
        PokerGame("Round " + to_string(i + 1) + "\n");

        shuffledCardDeck.cardShuffling();

        dealingCards(players, shuffledCardDeck);

        cout << "Dealer's cards" << endl;
        cout << playerCards(players[0]);
        cout << endl
             << endl;

        for (int i = 1; i < players.size(); i++)
        {
            if (!players[i].playingCheck)
                continue;

            cout << "Do you want to withdraw player " << i << " (y/n)? ";
            char c;
            cin >> c;

            if (c == 'y')
            {
                cout << "Player " << i << " withdrew his hand from this round" << endl;
                players[i].playingCheck = false;
            }
            else if (c != 'n')
            {
                cout << "Invalid input. Nothing has changed" << endl;
            }
            
        }

        ScoreUpdate(players);
        updateProbability(players);

        PokerGame("Cards are distributed to the players \n");
        PokerGame("Score card and the winning probability are as follows \n");
        PokerGame("(Consider player 0 as the dealer) \n");

        for (int i = 0; i < players.size(); i++)
        {
            string details = "";
            details += "\tPlayer " + to_string(i) + "\n";
            details += "\tCards: " + playerCards(players[i]) + "\n";
            details += "\tWinning probability is " + to_string(players[i].probabilityOfWinning) + "\n\n";

            PokerGame(details);
        }

        cout << endl
             << "Scores of each player" << endl
             << endl;

        for (int i = 1; i < players.size(); i++)
        {
            if (i == 0)
                continue;

            cout << "\tPlayer " << i << endl;
            cout << "\tScore card: " << players[i].totalScore << endl
                 << fixed << setprecision(2)
                 << "\tWinning probability: " << players[i].probabilityOfWinning << " %" << endl;

            if (!players[i].playingCheck)
            {
                cout << "\tPlayer is not playing" << endl;
                players[i].playingCheck = true;
                cout << endl;
            }
            else
            {
                cout << endl;
            }
        }

        int winnerMarks = 0;
        
        for (int i = 1; i < players.size(); i++)
        {
            if (players[i].totalScore > winnerMarks)
            {
                winnerMarks = players[i].totalScore;
                winner = i;
            }
        }
        cout << "Winner is player " << winner << endl;
        cout << endl;
            
             
        PokerGame("Winner is player " + to_string(winner) + "\n");
        
    }
    cout << "Winner of the Poker Game considering all rounds is player " << winner << endl;
    PokerGame("Winner of the Poker Game considering all rounds is player " + to_string(winner) + "\n");
}

int main()
{
    simulatePokerGame();

    cin.clear();
    cin.get();
    return 0;
}
