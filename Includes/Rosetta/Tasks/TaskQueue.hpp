// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_TASK_QUEUE_HPP
#define ROSETTASTONE_TASK_QUEUE_HPP

#include <Rosetta/Tasks/ITask.hpp>

#include <queue>
#include <stack>

namespace RosettaStone
{
//!
//! \brief TaskQueue class.
//!
//! This class stores several tasks into stack to run in order.
//!
class TaskQueue
{
 public:
    //! Default constructor.
    TaskQueue() = default;

    //! Returns the current queue.
    //! \return The current queue.
    std::queue<ITask*>& GetCurrentQueue();

    //! Returns flag that indicates task queue is empty.
    //! \return Flag that indicates task queue is empty.
    bool IsEmpty();

    //! Starts the event.
    void StartEvent();

    //! Ends the event.
    void EndEvent();

    //! Enqueues the task.
    //! \param task The task to enqueue.
    void Enqueue(ITask* task);

    //! Processes the task.
    //! \return The result of task processing.
    TaskStatus Process();

 private:
    std::stack<std::queue<ITask*>> m_eventStack;
    std::queue<ITask*> m_baseQueue;

    ITask* m_currentTask = nullptr;

    bool m_eventFlag = false;
};
}  // namespace RosettaStone

#endif  // ROSETTASTONE_TASK_QUEUE_HPP
