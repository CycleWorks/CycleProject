#pragma once

#include "Common/utils.hpp"
#include "HIR/scope.hpp"

namespace Cycle::HIR {
    struct FileModule {
        FileModule();

        ScopeHead* get_scope_head();
        const ScopeHead* get_scope_head() const;
    private:
        std::unique_ptr<ScopeHead> _scope_head;
    };
}
