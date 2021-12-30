#pragma once

#include <casbin/casbin_helpers.h>
// #include <casbin/casbin_types.h>
class PersistenceAdapter : public casbin::Adapter
{
    /**
     * LoadPolicy loads all policy rules from the storage.
     *
     * @param model the model.
     */
    virtual void LoadPolicy(const std::shared_ptr<casbin::Model> &model) = 0;

    /**
     * SavePolicy saves all policy rules to the storage.
     *
     * @param model the model.
     */
    virtual void SavePolicy(const std::shared_ptr<casbin::Model> &model) = 0;

    /**
     * AddPolicy adds a policy rule to the storage.
     * This is part of the Auto-Save feature.
     *
     * @param sec the section, "p" or "g".
     * @param p_type the policy type, "p", "p2", .. or "g", "g2", ..
     * @param rule the rule, like (sub, obj, act).
     */
    virtual void AddPolicy(std::string sec, std::string p_type, std::vector<std::string> rule) = 0;

    /**
     * RemovePolicy removes a policy rule from the storage.
     * This is part of the Auto-Save feature.
     *
     * @param sec the section, "p" or "g".
     * @param p_type the policy type, "p", "p2", .. or "g", "g2", ..
     * @param rule the rule, like (sub, obj, act).
     */
    virtual void RemovePolicy(std::string sec, std::string p_type, std::vector<std::string> rule) = 0;

    /**
     * RemoveFilteredPolicy removes policy rules that match the filter from the storage.
     * This is part of the Auto-Save feature.
     *
     * @param sec the section, "p" or "g".
     * @param p_type the policy type, "p", "p2", .. or "g", "g2", ..
     * @param field_index the policy rule's start index to be matched.
     * @param field_values the field values to be matched, value ""
     *                    means not to match this field.
     */
    virtual void RemoveFilteredPolicy(std::string sec, std::string ptype, int field_index, std::vector<std::string> field_values) = 0;
};