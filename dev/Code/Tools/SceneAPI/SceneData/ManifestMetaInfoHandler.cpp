/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/std/containers/unordered_set.h>
#include <AzToolsFramework/Debug/TraceContext.h>
#include <SceneAPI/SceneCore/Containers/Scene.h>
#include <SceneAPI/SceneCore/Utilities/Reporting.h>
#include <SceneAPI/SceneCore/Utilities/SceneGraphSelector.h>
#include <SceneAPI/SceneData/ManifestMetaInfoHandler.h>
#include <SceneAPI/SceneData/Groups/MeshGroup.h>
#include <SceneAPI/SceneData/Groups/SkeletonGroup.h>
#include <SceneAPI/SceneData/Groups/SkinGroup.h>
#include <SceneAPI/SceneData/Groups/AnimationGroup.h>
#include <SceneAPI/SceneData/Rules/BlendShapeRule.h>
#include <SceneAPI/SceneData/Rules/CommentRule.h>
#include <SceneAPI/SceneData/Rules/LodRule.h>
#include <SceneAPI/SceneData/Rules/MaterialRule.h>
#include <SceneAPI/SceneData/Rules/StaticMeshAdvancedRule.h>
#include <SceneAPI/SceneData/Rules/OriginRule.h>
#include <SceneAPI/SceneData/Rules/PhysicsRule.h>
#include <SceneAPI/SceneData/Rules/SkeletonProxyRule.h>
#include <SceneAPI/SceneData/Rules/SkinMeshAdvancedRule.h>

#if defined(MOTIONCANVAS_GEM_ENABLED)
#include <SceneAPI/SceneData/Groups/ActorGroup.h>
#include <SceneAPI/SceneData/Groups/EFXMotionGroup.h>
#include <SceneAPI/SceneData/Rules/EFXMeshRule.h>
#include <SceneAPI/SceneData/Rules/EFXSkinRule.h>
#include <SceneAPI/SceneData/Rules/EFXMotionCompressionSettingsRule.h>
#include <SceneAPI/SceneData/Rules/EFXMotionScaleRule.h>
#include <SceneAPI/SceneData/Rules/EFXActorScaleRule.h>
#endif

namespace AZ
{
    namespace SceneAPI
    {
        namespace SceneData
        {
            AZ_CLASS_ALLOCATOR_IMPL(ManifestMetaInfoHandler, SystemAllocator, 0)

            ManifestMetaInfoHandler::ManifestMetaInfoHandler()
            {
                BusConnect();
            }

            ManifestMetaInfoHandler::~ManifestMetaInfoHandler()
            {
                BusDisconnect();
            }

            void ManifestMetaInfoHandler::GetAvailableModifiers(ModifiersList& modifiers, const Containers::Scene& /*scene*/, 
                const DataTypes::IManifestObject& target)
            {
                AZ_TraceContext("Object Type", target.RTTI_GetTypeName());

                if (target.RTTI_IsTypeOf(DataTypes::IMeshGroup::TYPEINFO_Uuid()))
                {
                    const DataTypes::IMeshGroup* group = azrtti_cast<const DataTypes::IMeshGroup*>(&target);
                    modifiers.push_back(SceneData::CommentRule::TYPEINFO_Uuid());
                    const Containers::RuleContainer& rules = group->GetRuleContainerConst();

                    AZStd::unordered_set<Uuid> existingRules;
                    const size_t ruleCount = rules.GetRuleCount();
                    for (size_t i = 0; i < ruleCount; ++i)
                    {
                        existingRules.insert(rules.GetRule(i)->RTTI_GetType());
                    }
                    if (existingRules.find(SceneData::LodRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::LodRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::MaterialRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::MaterialRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::StaticMeshAdvancedRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::StaticMeshAdvancedRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::OriginRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::OriginRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::PhysicsRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::PhysicsRule::TYPEINFO_Uuid());
                    }
                }
                else if (target.RTTI_IsTypeOf(DataTypes::ISkinGroup::TYPEINFO_Uuid()))
                {
                    const DataTypes::ISkinGroup* group = azrtti_cast<const DataTypes::ISkinGroup*>(&target);
                    modifiers.push_back(SceneData::CommentRule::TYPEINFO_Uuid());
                    const Containers::RuleContainer& rules = group->GetRuleContainerConst();

                    AZStd::unordered_set<AZ::Uuid> existingRules;
                    const size_t ruleCount = rules.GetRuleCount();
                    for (size_t i = 0; i < ruleCount; ++i)
                    {
                        existingRules.insert(rules.GetRule(i)->RTTI_GetType());
                    }
                    if (existingRules.find(SceneData::BlendShapeRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::BlendShapeRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::LodRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::LodRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::MaterialRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::MaterialRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::SkinMeshAdvancedRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::SkinMeshAdvancedRule::TYPEINFO_Uuid());
                    }

                }
                else if (target.RTTI_IsTypeOf(DataTypes::ISkeletonGroup::TYPEINFO_Uuid()))
                {
                    const DataTypes::ISkeletonGroup* group = azrtti_cast<const DataTypes::ISkeletonGroup*>(&target);
                    modifiers.push_back(SceneData::CommentRule::TYPEINFO_Uuid());
                    const Containers::RuleContainer& rules = group->GetRuleContainerConst();

                    AZStd::unordered_set<AZ::Uuid> existingRules;
                    const size_t ruleCount = rules.GetRuleCount();
                    for (size_t i = 0; i < ruleCount; ++i)
                    {
                        existingRules.insert(rules.GetRule(i)->RTTI_GetType());
                    }

                    if (existingRules.find(SceneData::SkeletonProxyRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::SkeletonProxyRule::TYPEINFO_Uuid());
                    }
                }
                else if (target.RTTI_IsTypeOf(DataTypes::IAnimationGroup::TYPEINFO_Uuid()))
                {
                    modifiers.push_back(SceneData::CommentRule::TYPEINFO_Uuid());
                }
#if defined(MOTIONCANVAS_GEM_ENABLED)
                else if (target.RTTI_IsTypeOf(DataTypes::IActorGroup::TYPEINFO_Uuid()))
                {
                    modifiers.push_back(SceneData::CommentRule::TYPEINFO_Uuid());
                    const DataTypes::IActorGroup* group = azrtti_cast<const DataTypes::IActorGroup*>(&target);
                    const Containers::RuleContainer& rules = group->GetRuleContainerConst();

                    AZStd::unordered_set<AZ::Uuid> existingRules;
                    const size_t ruleCount = rules.GetRuleCount();
                    for (size_t i = 0; i < ruleCount; ++i)
                    {
                        existingRules.insert(rules.GetRule(i)->RTTI_GetType());
                    }
                    if (existingRules.find(SceneData::EFXMeshRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::EFXMeshRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::MaterialRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::MaterialRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::EFXSkinRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::EFXSkinRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::EFXActorScaleRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::EFXActorScaleRule::TYPEINFO_Uuid());
                    }
                }
                else if (target.RTTI_IsTypeOf(DataTypes::IEFXMotionGroup::TYPEINFO_Uuid()))
                {
                    modifiers.push_back(SceneData::CommentRule::TYPEINFO_Uuid());
                    const DataTypes::IEFXMotionGroup* group = azrtti_cast<const DataTypes::IEFXMotionGroup*>(&target);
                    const Containers::RuleContainer& rules = group->GetRuleContainerConst();

                    AZStd::unordered_set<AZ::Uuid> existingRules;
                    const size_t ruleCount = rules.GetRuleCount();
                    for (size_t i = 0; i < ruleCount; ++i)
                    {
                        existingRules.insert(rules.GetRule(i)->RTTI_GetType());
                    }
                    if (existingRules.find(SceneData::EFXMotionCompressionSettingsRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::EFXMotionCompressionSettingsRule::TYPEINFO_Uuid());
                    }
                    if (existingRules.find(SceneData::EFXMotionScaleRule::TYPEINFO_Uuid()) == existingRules.end())
                    {
                        modifiers.push_back(SceneData::EFXMotionScaleRule::TYPEINFO_Uuid());
                    }
                }
#endif
            }

            void ManifestMetaInfoHandler::InitializeObject(const Containers::Scene& scene, DataTypes::IManifestObject& target)
            {
                if (target.RTTI_IsTypeOf(DataTypes::IPhysicsRule::TYPEINFO_Uuid()))
                {
                    DataTypes::IPhysicsRule* rule = azrtti_cast<DataTypes::IPhysicsRule*>(&target);
                    Utilities::SceneGraphSelector::UnselectAll(scene.GetGraph(), rule->GetSceneNodeSelectionList());
                }
            }
        } // namespace SceneData
    } // namespace SceneAPI
} // namespace AZ