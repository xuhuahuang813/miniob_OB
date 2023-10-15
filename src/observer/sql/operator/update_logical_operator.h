//
// Created by hxh
//

#pragma once

#include <vector>
#include "sql/operator/logical_operator.h"
#include "sql/stmt/update_stmt.h"
#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"

/**
 * @brief 插入update算子
 * @ingroup UpdateOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, UpdateStmt *update_stmt);
  virtual ~UpdateLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

  Table      *table() const { return table_; }
  UpdateStmt *update_stmt() const { return update_stmt_; }

private:
  Table      *table_       = nullptr;
  UpdateStmt *update_stmt_ = nullptr;
};