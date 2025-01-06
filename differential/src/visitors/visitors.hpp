#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "expr.hpp"
#include <vector>

namespace Visitors{
    [[nodiscard]] std::string AstPrinter(const Expr::Expr& expr, size_t indent_size = 0);
    [[nodiscard]] std::string ExprToLatex(const Expr::Expr& expr);
    std::nullptr_t TreeDestroyer(Expr::Expr* root);

    using IdList = std::unordered_set<Expr::Identifier, Expr::IdentifierHasher, Expr::IdentifierEqual>;
    class IdentifierLister{
      private:
        IdList identifiers;
        void search_identifiers(const Expr::Expr* root) noexcept;
      public:
        IdList get_identifiers(const Expr::Expr* root) noexcept;
    };

    Expr::Expr* TreeCopier(const Expr::Expr* root);
    std::vector<Expr::Expr*> CallArgsCopier(const std::vector<Expr::Expr*>& expr);
    Expr::Expr* NodeCopier(const Expr::Expr& node);

    Expr::Expr* Differentiator(const Expr::Expr* root, const Expr::Identifier& diff_id);
    Expr::Expr* ArithmeticOptimizer(Expr::Expr* root);
}


#endif
