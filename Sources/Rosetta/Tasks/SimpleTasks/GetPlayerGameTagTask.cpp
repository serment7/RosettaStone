// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Tasks/SimpleTasks/GetPlayerGameTagTask.hpp>

namespace RosettaStone::SimpleTasks
{
GetPlayerGameTagTask::GetPlayerGameTagTask(GameTag tag) : m_gameTag(tag)
{
    // Do nothing
}

TaskStatus GetPlayerGameTagTask::Impl(Player* player)
{
    player->game->taskStack.num[0] = player->GetGameTag(m_gameTag);
    return TaskStatus::COMPLETE;
}

ITask* GetPlayerGameTagTask::CloneImpl()
{
    return new GetPlayerGameTagTask(m_gameTag);
}
}  // namespace RosettaStone::SimpleTasks
