//
// Created by hxh
//

#pragma once

#include <vector>
#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"

class UpdateStmt;

/**
 * @brief update物理算子
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
  UpdatePhysicalOperator(Table *table, UpdateStmt *update_stmt): table_(table), update_stmt_(update_stmt){}

  virtual ~UpdatePhysicalOperator() = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::UPDATE;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  Table *table_ = nullptr;
  UpdateStmt *update_stmt_ = nullptr;
  Trx *trx_ = nullptr;
};