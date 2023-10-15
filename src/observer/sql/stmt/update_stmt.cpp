/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "common/log/log.h"
#include "storage/table/table.h"
#include "storage/db/db.h"


// UpdateStmt::UpdateStmt(Table *table, Value *values, int value_amount)
//     : table_(table), values_(values), value_amount_(value_amount)
// {}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{

  /*获取table，参考insert_stmt.cpp*/
  const char *table_name = update.relation_name.c_str();
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("invalid argument, dp=%p, table_name=%p", db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  /*检查table是否存在*/
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  /*获取阈和值*/
  std::vector<const FieldMeta *> field_metas_;
  std::vector<Value>             values;
  for (const auto &tuple : update.update_tuples) {
    values.emplace_back(tuple.value);
    const FieldMeta *field_meta = table->table_meta().field(tuple.attribute_name.c_str());
    if (nullptr == field_meta) {
      LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), tuple.attribute_name.c_str());
      return RC::SCHEMA_FIELD_MISSING;
    }
    field_metas_.emplace_back(field_meta);
  }

  /*处理where条件，参考select_stmt.cpp*/
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(table_name, table));
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(db, table, &table_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  UpdateStmt *update_stmt = new UpdateStmt();
  update_stmt->table_ = table;
  update_stmt->field_metas_.swap(field_metas_);
  update_stmt->values.swap(values);
  update_stmt->filter_stmt_ = filter_stmt;
  
  stmt = update_stmt;
  return RC::SUCCESS;
}
