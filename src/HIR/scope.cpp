#include "HIR/scope.hpp"
#include "Common/errors.hpp"
#include <memory>
#include <optional>

using namespace Cycle::HIR;

// BaseScopeContext:
BaseScopeContext::BaseScopeContext(const BaseScope* this_scope):
    _this_scope(this_scope)
{}

const BaseScope* BaseScopeContext::get_this_scope() const {
    return _this_scope;
}

bool BaseScopeContext::has_symbol(std::string_view symbol_name) const {
    return _symbols[symbol_name] != nullptr;
}

Symbols::Symbol* BaseScopeContext::add_symbol(std::unique_ptr<Symbols::Symbol>&& symbol){
    if (this->has_symbol(symbol->get_symbol_name())){
        throw InternalError("Tried to add symbol '{}' to scope but it already exists", symbol->get_symbol_name());
    }
    std::string_view symbol_name_view = symbol->get_symbol_name();
    Symbols::Symbol* symbol_ptr = symbol.get();

    _symbols[symbol_name_view] = std::move(symbol);
    return symbol_ptr;
}

// GlobalScopeContext:
GlobalScopeContext::GlobalScopeContext(const BaseScope* this_scope):
    BaseScopeContext(this_scope)
{}

std::unique_ptr<BaseScopeContext> GlobalScopeContext::make_child_context() const {
    return std::unique_ptr<GlobalScopeContext>(new GlobalScopeContext(this->get_this_scope()));
}

// FunctionScopeContext:
FunctionScopeContext::FunctionScopeContext(const BaseScope* this_scope):
    BaseScopeContext(this_scope)
{}

std::unique_ptr<BaseScopeContext> FunctionScopeContext::make_child_context() const {
    return std::unique_ptr<FunctionScopeContext>(new FunctionScopeContext(this->get_this_scope()));
}

// BaseScope:
BaseScope::BaseScope(std::optional<const BaseScope*> maybe_parent_scope):
    _parent_scope(maybe_parent_scope),
    _scope_context(_make_context_from_parent(maybe_parent_scope))
{}

BaseScope::BaseScope(std::optional<const BaseScope*> maybe_parent_scope, std::unique_ptr<BaseScopeContext> override_scope_context):
    _parent_scope(maybe_parent_scope),
    _scope_context(std::move(override_scope_context))
{}

bool BaseScope::has_parent_scope() const {
    return _parent_scope.has_value();
}

std::optional<const BaseScope*> BaseScope::get_parent_scope() const {
    return _parent_scope;
}

const BaseScopeContext* BaseScope::get_scope_context() const {
    return _scope_context.get();
}

BaseScopeContext* BaseScope::get_scope_context(){
    return _scope_context.get();
}

std::unique_ptr<BaseScopeContext> BaseScope::_make_context_from_parent(std::optional<const BaseScope*> maybe_parent_scope){
    if (!maybe_parent_scope.has_value()){
        return std::unique_ptr<GlobalScopeContext>(new GlobalScopeContext(this));
    }
    const BaseScopeContext* parent_scope_context = maybe_parent_scope.value()->get_scope_context();
    return parent_scope_context->make_child_context();
}

// Scope:
Scope::Scope(const BaseScope* parent_scope):
    BaseScope(parent_scope)
{}

Scope::Scope(const BaseScope* parent_scope, std::unique_ptr<BaseScopeContext> override_scope_context):
    BaseScope(parent_scope)
{}

// ScopeHead:
ScopeHead::ScopeHead():
    BaseScope(std::nullopt)
{}
