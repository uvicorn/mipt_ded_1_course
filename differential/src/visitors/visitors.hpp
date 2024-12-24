#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "expr.hpp"


namespace Visitors{

    [[nodiscard]] std::string AstPrinter(const Expr::Expr& expr, size_t indent_size=0);

    using IdList = std::unordered_set<Expr::Identifier, Expr::IdentifierHasher, Expr::IdentifierEqual>;
    class IdentifierLister{
      private:
        IdList identifiers;
        void search_identifiers(const Expr::Expr* root) noexcept;
      public:
        IdList get_identifiers(const Expr::Expr* root) noexcept;
    };
}


#endif
