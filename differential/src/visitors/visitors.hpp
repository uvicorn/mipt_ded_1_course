#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "expr.hpp"


namespace Visitors{
    [[nodiscard]] std::string AstPrinter(const Expr::Expr& expr, size_t indent_size = 0);
    [[nodiscard]] std::nullptr_t TreeDestroyer(Expr::Expr* root);

    using IdList = std::unordered_set<Expr::Identifier, Expr::IdentifierHasher, Expr::IdentifierEqual>;
    class IdentifierLister{
      private:
        IdList identifiers;
        void search_identifiers(const Expr::Expr* root) noexcept;
      public:
        IdList get_identifiers(const Expr::Expr* root) noexcept;
    };

    Expr::Expr* TreeCopier(const Expr::Expr* root);
    Expr::Expr* NodeCopier(Expr::Expr& node);
}


#endif
