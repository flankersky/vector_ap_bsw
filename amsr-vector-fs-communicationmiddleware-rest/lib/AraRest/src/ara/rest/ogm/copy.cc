/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  object.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>

#include "ara/rest/ogm/copy.h"
#include "ara/rest/ogm/visit.h"

namespace ara {

namespace rest {

namespace ogm {

namespace details {

// TODO(x): allocator is not passed around or used here yet

ara::rest::Pointer<Node> CopyImpl(const Node* u) {
  Pointer<Node> p{};
  VisitImpl(cast<Node>(u),  // erase precise type
            [&p](const Field* u) { p.reset(Field::Make(u->GetName().data(), Copy(u->GetValue())).release()); },
            [&p](const Int* u) { p.reset(Int::Make(u->GetValue()).release()); },
            [&p](const Real* u) { p.reset(Real::Make(u->GetValue()).release()); },
            [&p](const String* u) { p.reset(String::Make(u->GetValue()).release()); },
            [&p](const Uuid* u) { p.reset(Uuid::Make(u->GetValue()).release()); },
            [&p](const Uri* u) { p.reset(Uri::Make(u->GetValue()).release()); },
            [&p](const Array* u) {
              auto a = Array::Make();
              for (auto& e : u->GetValues()) {
                a->Append(Copy(&e));  // recurse
              }
              p.reset(a.release());
            },
            [&p](const Object* u) {
              auto o = Object::Make();
              for (auto& f : u->GetFields()) {
                o->Insert(Copy(&f));  // recurse
              }
              p.reset(o.release());
            });

  assert(p);
  return p;
}

}  // namespace details
}  // namespace ogm
}  // namespace rest
}  // namespace ara
