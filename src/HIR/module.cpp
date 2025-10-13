#include "HIR/module.hpp"
#include <memory>

using namespace Cycle::HIR;

FileModule::FileModule():
    _scope_head(std::unique_ptr<ScopeHead>(new ScopeHead()))
{}

ScopeHead* FileModule::get_scope_head(){
    return _scope_head.get();
}

const ScopeHead* FileModule::get_scope_head() const {
    return _scope_head.get();
}
