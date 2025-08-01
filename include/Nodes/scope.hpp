#pragma once

#include "Nodes/node.hpp"
#include "Symbols/symbol.hpp"
#include "Common/utils.hpp"
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Cycle::Nodes {
    struct BaseScope;

    // Scope contexts:
    struct BaseScopeContext : public AbstractClass {
        explicit BaseScopeContext(const BaseScope* this_scope);
        const BaseScope* get_this_scope() const;

        bool has_symbol(const std::string& symbol_name) const;
        Symbols::Symbol* add_symbol(std::unique_ptr<Symbols::Symbol>&& symbol);

        virtual std::unique_ptr<BaseScopeContext> make_child_context() const = 0;
    private:
        const BaseScope* _this_scope;
        mutable std::unordered_map<std::string_view, std::unique_ptr<Symbols::Symbol>> _symbols;
    };

    struct GlobalScopeContext : public BaseScopeContext {
        explicit GlobalScopeContext(const BaseScope* this_scope);

        std::unique_ptr<BaseScopeContext> make_child_context() const override;
    private:
        void polymorphism() const override {}
    };

    struct FunctionScopeContext : public BaseScopeContext {
        explicit FunctionScopeContext(const BaseScope* this_scope);

        std::unique_ptr<BaseScopeContext> make_child_context() const override;
    private:
        // TODO Implement later:
        // std::vector<std::unique_ptr<BaseNode>> _child_nodes;

        void polymorphism() const override {}
    };

    // Scopes:
    struct BaseScope : public BaseNode {
        explicit BaseScope(std::optional<const BaseScope*> parent_scope);
        explicit BaseScope(std::optional<const BaseScope*> parent_scope, std::unique_ptr<BaseScopeContext> override_scope_context);

        bool has_parent_scope() const;
        std::optional<const BaseScope*> get_parent_scope() const;

        const BaseScopeContext* get_scope_context() const;
        BaseScopeContext* get_scope_context();
    private:
        std::unique_ptr<BaseScopeContext> _make_context_from_parent(std::optional<const BaseScope*> parent_scope);

        std::optional<const BaseScope*> _parent_scope;
        std::unique_ptr<BaseScopeContext> _scope_context;
    };

    struct Scope : public BaseScope {
        explicit Scope(const BaseScope* maybe_parent_scope);
        explicit Scope(const BaseScope* maybe_parent_scope, std::unique_ptr<BaseScopeContext> override_scope_context);
    private:
        void polymorphism() const override {}
    };

    struct ScopeHead : public BaseScope {
        explicit ScopeHead();
    private:
        void polymorphism() const override {}
    };

    // Factories:
    struct GlobalScopeContextFactory : BasicFactory<GlobalScopeContext> {};
    struct FunctionScopeContextFactory : BasicFactory<FunctionScopeContext> {};
    struct ScopeFactory : BasicFactory<Scope> {};
    struct ScopeHeadFactory : BasicFactory<ScopeHead> {};
}
