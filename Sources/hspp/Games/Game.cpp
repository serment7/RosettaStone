// Copyright (c) 2018 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include <hspp/Actions/Draw.hpp>
#include <hspp/Actions/Generic.hpp>
#include <hspp/Cards/Cards.hpp>
#include <hspp/Games/Game.hpp>
#include <hspp/Games/GameManager.hpp>

#include <effolkronium/random.hpp>

#include <algorithm>

using Random = effolkronium::random_static;

namespace Hearthstonepp
{
Game::Game(GameConfig& gameConfig) : m_gameConfig(gameConfig)
{
    // Add hero and hero power
    GetPlayer1().AddHeroAndPower(
        Cards::GetInstance().GetHeroCard(gameConfig.player1Class),
        Cards::GetInstance().GetDefaultHeroPower(gameConfig.player1Class));
    GetPlayer2().AddHeroAndPower(
        Cards::GetInstance().GetHeroCard(gameConfig.player2Class),
        Cards::GetInstance().GetDefaultHeroPower(gameConfig.player2Class));

    // Set opponent player
    GetPlayer1().SetOpponent(&GetPlayer2());
    GetPlayer2().SetOpponent(&GetPlayer1());
}

Player& Game::GetPlayer1()
{
    return m_players[0];
}

Player& Game::GetPlayer2()
{
    return m_players[1];
}

Player& Game::GetCurrentPlayer()
{
    return *m_currentPlayer;
}

Player& Game::GetOpponentPlayer()
{
    return m_currentPlayer->GetOpponent();
}

void Game::BeginFirst()
{
    // Set next step
    nextStep = Step::BEGIN_SHUFFLE;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::BeginShuffle()
{
    // Shuffle cards in deck
    if (m_gameConfig.doShuffle)
    {
        GetPlayer1().GetDeck().Shuffle();
        GetPlayer2().GetDeck().Shuffle();
    }

    // Set next step
    nextStep = Step::BEGIN_DRAW;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::BeginDraw()
{
    for (auto& p : m_players)
    {
        // Draw 3 cards
        Generic::Draw(p);
        Generic::Draw(p);
        Generic::Draw(p);

        if (&p != m_firstPlayer)
        {
            // Draw 4th card for second player
            Generic::Draw(p);

            // Give "The Coin" card to second player
            Card coin = Cards::GetInstance().FindCardByID("GAME_005");
            p.GetHand().AddCard(*Entity::GetFromCard(p, std::move(coin)));
        }
    }

    // Set next step
    nextStep =
        m_gameConfig.skipMulligan ? Step::MAIN_BEGIN : Step::BEGIN_MULLIGAN;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::BeginMulligan()
{
    // Do nothing
}

void Game::MainBegin()
{
    // Set next step
    nextStep = Step::MAIN_READY;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainReady()
{
    // Reset the number of attacked
    for (auto& p : m_players)
    {
        // Hero
        p.GetHero()->numAttacked = 0;
        // Field
        for (auto& m : p.GetField().GetAllMinions())
        {
            m->numAttacked = 0;
        }
    }

    // Reset exhaust for current player
    auto& curPlayer = GetCurrentPlayer();
    // Hero
    curPlayer.GetHero()->SetGameTag(GameTag::EXHAUSTED, 0);
    // Weapon
    if (curPlayer.GetHero()->weapon != nullptr)
    {
        curPlayer.GetHero()->weapon->SetGameTag(GameTag::EXHAUSTED, 0);
    }
    // Hero power
    curPlayer.GetHero()->heroPower->SetGameTag(GameTag::EXHAUSTED, 0);
    // Field
    for (auto& m : curPlayer.GetField().GetAllMinions())
    {
        m->SetGameTag(GameTag::EXHAUSTED, 0);
    }

    // Set next step
    nextStep = Step::MAIN_START_TRIGGERS;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainStartTriggers()
{
    // Set next step
    nextStep = Step::MAIN_RESOURCE;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainResource()
{
    auto& curPlayer = GetCurrentPlayer();

    // Add mana crystal to current player
    Generic::ChangeManaCrystal(curPlayer, 1, false);

    // Reset current mana
    curPlayer.currentMana = curPlayer.maximumMana;

    // Set next step
    nextStep = Step::MAIN_DRAW;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainDraw()
{
    // Draw a card for current player
    Generic::Draw(GetCurrentPlayer());

    // Set next step
    nextStep = Step::MAIN_START;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainStart()
{
    // Set next step
    nextStep = Step::MAIN_ACTION;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainEnd()
{
    // Set next step
    nextStep = Step::MAIN_CLEANUP;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainCleanUp()
{
    auto& curPlayer = GetCurrentPlayer();

    // Unfreeze all characters they control that are Frozen, don't have
    // summoning sickness (or do have Charge) and have not attacked that turn
    // Hero
    if (curPlayer.GetHero()->GetGameTag(GameTag::FROZEN) == 1 &&
        curPlayer.GetHero()->numAttacked == 0)
    {
        curPlayer.GetHero()->SetGameTag(GameTag::FROZEN, 0);
    }
    // Field
    for (auto& m : curPlayer.GetField().GetAllMinions())
    {
        if (m->GetGameTag(GameTag::FROZEN) == 1 && m->numAttacked == 0 &&
            m->GetGameTag(GameTag::EXHAUSTED) == 0)
        {
            m->SetGameTag(GameTag::FROZEN, 0);
        }
    }

    // Set next step
    nextStep = Step::MAIN_NEXT;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::MainNext()
{
    // Set player for next turn
    m_currentPlayer = &m_currentPlayer->GetOpponent();

    // Count next turn
    m_turn++;

    // Set next step
    nextStep = Step::MAIN_READY;
    GameManager::ProcessNextStep(*this, nextStep);
}

void Game::FinalWrapUp()
{
    // Do nothing
}

void Game::FinalGameOver()
{
    // Do nothing
}

void Game::StartGame()
{
    // Reverse card order in deck
    if (!m_gameConfig.doShuffle)
    {
        std::reverse(m_gameConfig.player1Deck.begin(),
                     m_gameConfig.player1Deck.end());
        std::reverse(m_gameConfig.player2Deck.begin(),
                     m_gameConfig.player2Deck.end());
    }

    // Set up decks
    for (auto& card : m_gameConfig.player1Deck)
    {
        Entity* entity = Entity::GetFromCard(GetPlayer1(), std::move(card));
        GetPlayer1().GetDeck().AddCard(*entity);
    }
    for (auto& card : m_gameConfig.player2Deck)
    {
        Entity* entity = Entity::GetFromCard(GetPlayer2(), std::move(card));
        GetPlayer2().GetDeck().AddCard(*entity);
    }

    // Determine first player
    switch (m_gameConfig.startPlayer)
    {
        case PlayerType::RANDOM:
        {
            auto val = Random::get(0, 1);
            m_firstPlayer = &m_players[val];
            break;
        }
        case PlayerType::PLAYER1:
            m_firstPlayer = &m_players[0];
            break;
        case PlayerType::PLAYER2:
            m_firstPlayer = &m_players[1];
            break;
    }
    m_currentPlayer = m_firstPlayer;

    // Set first turn
    m_turn = 1;

    // Set next step
    nextStep = Step::BEGIN_FIRST;
    GameManager::ProcessNextStep(*this, nextStep);
}
}  // namespace Hearthstonepp