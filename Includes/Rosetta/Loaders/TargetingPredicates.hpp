// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#ifndef ROSETTASTONE_TARGETING_PREDICATES_HPP
#define ROSETTASTONE_TARGETING_PREDICATES_HPP

#include <Rosetta/Enums/CardEnums.hpp>

#include <functional>

namespace RosettaStone
{
class Card;
class Player;
class Character;

using TargetingPredicate = std::function<bool(Character*)>;
using AvailabilityPredicate = std::function<bool(Player*, Card*)>;

//!
//! \brief TargetingPredicates class.
//!
//! This class includes utility methods for targeting predicate.
//!
class TargetingPredicates
{
 public:
    //! Predicate wrapper for checking the target requires that
    //! it has Race::MURLOC.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqMurlocTarget();

    //! Predicate wrapper for checking the target requires that
    //! it has Race::DEMON.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqDemonTarget();

    //! Predicate wrapper for checking the target requires that
    //! it has Race::MECHANICAL.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqMechanicalTarget();

    //! Predicate wrapper for checking the target requires that
    //! it has Race::ELEMENTAL.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqElementalTarget();

    //! Predicate wrapper for checking the target requires that
    //! it has Race::BEAST.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqBeastTarget();

    //! Predicate wrapper for checking the target requires that
    //! it has Race::TOTEM.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqTotemTarget();

    //! Predicate wrapper for checking the target requires that
    //! it has Race::PIRATE.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqPirateTarget();

    //! Predicate wrapper for checking the target requires that
    //! it has Race::DRAGON.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqDragonTarget();

    //! Predicate wrapper for checking the target requires that it has \p race.
    //! \param race The value of race.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqTargetWithRace(Race race);

    //! Predicate wrapper for checking the target requires that
    //! it is damaged.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqDamagedTarget();

    //! Predicate wrapper for checking the target requires that
    //! it is undamaged.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqUndamagedTarget();

    //! Predicate wrapper for checking the target requires that
    //! the maximum attack is \p value.
    //! \param value The value of maximum attack.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqTargetMaxAttack(int value);

    //! Predicate wrapper for checking the target requires that
    //! the minimum attack is \p value.
    //! \param value The value of minimum attack.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqTargetMinAttack(int value);

    //! Predicate wrapper for checking the target requires combo active.
    //! \return Generated AvailabilityPredicate for intended purpose.
    static AvailabilityPredicate ReqTargetForCombo();

    //! Predicate wrapper for checking the target requires that it has taunt.
    //! \return Generated TargetingPredicate for intended purpose.
    static TargetingPredicate ReqMustTargetTaunter();
};
}  // namespace RosettaStone

#endif  // ROSETTASTONE_TARGETING_PREDICATES_HPP
