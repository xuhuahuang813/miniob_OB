//
// Created by hxh
//

#include "sql/operator/update_logical_operator.h"

UpdateLogicalOperator::UpdateLogicalOperator(Table *table, UpdateStmt *update_stmt)
    : table_(table), update_stmt_(update_stmt)
{
}