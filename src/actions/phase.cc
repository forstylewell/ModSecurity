/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */

#include "actions/phase.h"

#include <iostream>
#include <string>

#include "modsecurity/assay.h"
#include "src/rule.h"
#include "src/utils.h"
#include "modsecurity/modsecurity.h"

namespace ModSecurity {
namespace actions {

Phase::Phase(std::string action)
    : Action(action) {
    this->action_kind = ConfigurationKind;
    std::string a = action;
    a.erase(0, 6);
    if (a.at(0) == '\'') {
        a.erase(0, 1);
        a.pop_back();
    }

    try {
        this->phase = std::stoi(a);
    } catch (...) {
        this->phase = 0;
        if (tolower(a) == "request") {
            this->phase = this->phase + ModSecurity::Phases::RequestHeadersPhase;
        }
        if (tolower(a) == "response") {
            this->phase = this->phase + ModSecurity::Phases::ResponseBodyPhase;
        }
        if (tolower(a) == "logging") {
            this->phase = this->phase + ModSecurity::Phases::LoggingPhase;
        }
    }

    if (this->phase == 0) {
      /* Phase 0 is something new, we want to use as ConnectionPhase */
      this->phase = ModSecurity::Phases::ConnectionPhase;
    } else {
      /* Otherwise we want to shift the rule to the correct phase */
      this->phase = phase + ModSecurity::Phases::RequestHeadersPhase - 1;
    }
}

bool Phase::evaluate(Rule *rule, Assay *assay) {
    rule->phase = this->phase;
    return true;
}

}  // namespace actions
}  // namespace ModSecurity
