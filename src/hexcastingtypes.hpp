#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

enum PatternType {
    akashas_distillation,
    akashas_gambit,
    altiora,
    additive_distillation,
    alidades_purification,
    anchorites_flight,
    archers_distillation,
    architects_distillation,
    arcs_reflection,
    augurs_exaltation,
    augurs_purification,
    auditors_purification,
    auditors_reflection,
    assessors_purification,
    assessors_reflection,
    aviators_purification,
    banish_sentinel,
    axial_purification,
    black_suns_nadir,
    black_suns_zenith,
    blink,
    blue_suns_nadir,
    blue_suns_zenith,
    bookkeepers_gambit,
    break_block,
    ceiling_purification,
    charons_gambit,
    casters_glamour,
    chroniclers_gambit,
    chroniclers_prfn,
    circle_reflection,
    compass_purification,
    compass_purification_II,
    conjunction_distillation,
    conjure_light,
    conjure_block,
    consideration,
    cosine_purification,
    craft_artifact,
    craft_cypher,
    craft_trinket,
    craft_phial,
    create_water,
    create_lava,
    derivation_decomposition,
    destroy_liquid,
    dioscuri_gambit,
    disjunction_distillation,
    division_distillation,
    dispel_rain,
    entropy_reflection,
    eulers_reflection,
    edify_sapling,
    entity_prfn,
    entity_prfn_animal,
    entity_prfn_item,
    entity_prfn_living,
    entity_prfn_monster,
    entity_prfn_player,
    equality_distillation,
    erase_item,
    extinguish_area,
    excisors_distillation,
    exclusion_distillation,
    explosion,
    false_reflection,
    fireball,
    fishermans_gambit,
    fishermans_gambit_II,
    flocks_disintegration,
    flocks_gambit,
    flocks_reflection,
    flay_mind,
    floor_purification,
    gemini_decomposition,
    gemini_gambit,
    green_suns_nadir,
    green_suns_zenith,
    greater_fold_reflection,
    greater_teleport,
    hermes_gambit,
    huginns_gambit,
    ignite,
    inequality_distillation,
    impulse,
    integration_distillation,
    inverse_cosine_purification,
    inverse_sine_purification,
    inverse_tangent_distillation,
    inverse_tangent_purification,
    internalize_pigment,
    introspection,
    iris_gambit,
    jesters_gambit,
    length_purification,
    locate_sentinel,
    lesser_fold_reflection,
    locators_distillation,
    lodestone_reflection,
    logarithmic_distillation,
    make_note,
    maximus_distillation,
    maximus_distillation_II,
    minds_reflection,
    minimus_distillation,
    minimus_distillation_II,
    modulus_distillation,
    multiplicative_distillation,
    muninns_reflection,
    negation_purification,
    nullary_reflection,
    numerical_reflection,
    overgrow,
    pace_purification,
    place_block,
    power_distillation,
    prospectors_gambit,
    retrograde_purification,
    recharge_item,
    retrospection,
    reveal,
    red_suns_nadir,
    red_suns_zenith,
    rotation_gambit,
    rotation_gambit_II,
    scouts_distillation,
    scribes_gambit,
    scribes_reflection,
    selection_distillation,
    selection_exaltation,
    speakers_decomposition,
    speakers_distillation,
    singles_purification,
    stadiometers_prfn,
    subtractive_distillation,
    sine_purification,
    swindlers_gambit,
    summon_lightning,
    summon_rain,
    summon_greater_sentinel,
    summon_sentinel,
    surgeons_exaltation,
    tangent_purification,
    thanatos_reflection,
    thoths_gambit,
    true_reflection,
    uniqueness_purification,
    vacant_reflection,
    vector_disintegration,
    vector_exaltation,
    vector_reflection_NX,
    vector_reflection_NY,
    vector_reflection_NZ,
    vector_reflection_PX,
    vector_reflection_PY,
    vector_reflection_PZ,
    vector_reflection_zero,
    wayfarers_flight,
    wayfind_sentinel,
    waystone_reflection,
    white_suns_nadir,
    white_suns_zenith,
    zone_dstl_animal,
    zone_dstl_any,
    zone_dstl_item,
    zone_dstl_living,
    zone_dstl_monster,
    zone_dstl_non_animal,
    zone_dstl_non_item,
    zone_dstl_non_living,
    zone_dstl_non_monster,
    zone_dstl_non_player,
    zone_dstl_player,
    embedded_iota,
    num_patterns
};

struct Pattern {
    PatternType type;
    std::optional<std::string> value;
};

struct Iota {
    Iota(std::variant<double, bool, std::monostate, std::vector<double>, std::vector<Iota>, std::string> value_)
        :value(value_), has_value(true) {}

    Iota()
        :has_value(false) {}

    std::variant<double, bool, std::monostate, std::vector<double>, std::vector<Iota>, std::string> value;
    bool has_value = true;

    Pattern get_iota_pattern() const {
        struct IotaVisitor {
            Pattern& output;

            IotaVisitor(Pattern& output_)
                :output(output_) {}

            void operator()(const double d)
            {
                output = Pattern {.type = embedded_iota, .value = std::to_string(d)};
            }

            void operator()(const bool b)
            {
                output = Pattern {.type = embedded_iota, .value = b ? "True" : "False"};
            }

            void operator()(const std::monostate n)
            {
                output = Pattern {.type = embedded_iota, .value = "Null"};
            }

            void operator()(const std::vector<double> v)
            {
                output = Pattern {.type = embedded_iota,
                    .value = std::string("(") + std::to_string(v[0]) + ", " + std::to_string(v[1]) + ", " + std::to_string(v[2]) + ')'};
            }

            void operator()(const std::vector<Iota> l)
            {
                // Start list
                std::string val = "[";

                // Generate value of each iota in list
                for (int i = 0; i < l.size(); ++i)
                {
                    val += l[i].get_iota_pattern().value.value();

                    // Add commas between iotas in list
                    if (i < l.size() - 1)
                    {
                        val += ", ";
                    }
                }

                // End list
                val += "]";

                output = Pattern{.type = embedded_iota, .value = val};
            }

            void operator()(const std::string i)
            {
                output = Pattern{.type = embedded_iota, .value = i};
            }
        };

        Pattern output;
        IotaVisitor visitor = IotaVisitor(output);
        std::visit(visitor, value);

        return output;
    }
};

// Iota comparison operator
inline bool operator==(const Iota& lhs, const Iota& rhs)
{
    if (!lhs.has_value && !rhs.has_value)
    {
        return true;
    }

    return lhs.value == rhs.value;
}