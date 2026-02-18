#define _USE_MATH_DEFINES

#include <vector>
#include <unordered_set>
#include <numeric>
#include <math.h>

#include "generation.hpp"

struct Iota {
    std::variant<double, bool, void*, std::vector<double>, std::vector<Iota>, std::string> value;
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

            void operator()(const void* n)
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
bool operator==(const Iota& lhs, const Iota& rhs)
{
    if (!lhs.has_value && !rhs.has_value)
    {
        return true;
    }

    return lhs.value == rhs.value;
}

struct InbuiltFunc {
    // The name of the function
    std::vector<std::string> names;

    // Whether the function is void or not
    bool is_void;

    // Whether the function is a member func or not
    bool is_member;

    // The number of parameters the inbuilt function takes
    int num_params;

    // Patterns used in the function
    std::vector<std::variant<PatternType, Pattern>> func_patterns;

    // Function pointer for evaluating iotas during compile time
    Iota (*eval)(std::vector<Iota>) = nullptr;

    // Get the list of patterns
    std::vector<Pattern> get_patterns()
    {
        struct PatternVisitor {
            Pattern& output;

            PatternVisitor (Pattern& output_)
                :output(output_) {}

            void operator()(const PatternType t)
            {
                output = Pattern{t};
            }

            void operator()(const Pattern p)
            {
                output = p;
            }
        };

        std::vector<Pattern> patterns = std::vector<Pattern>();
        patterns.reserve(func_patterns.size());

        Pattern output;
        PatternVisitor visitor = PatternVisitor(output);

        for (std::variant<PatternType, Pattern> var : func_patterns)
        {
            std::visit(visitor, var);
            patterns.push_back(output);
        }

        return patterns;
    }
};

std::vector<InbuiltFunc> inbuilt_funcs = {
    // Void Non-Member
    InbuiltFunc {.names = {"write"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {scribes_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"write"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {chroniclers_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"write_akashic"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {akashas_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"print"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {reveal, Pattern{bookkeepers_gambit, "v"}},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute_unsafe_no_ret"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {hermes_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute_unsafe_no_ret"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {jesters_gambit, hermes_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"mine"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {break_block},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_weakness"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {white_suns_nadir},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_levitation"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {blue_suns_nadir},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_withering"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {black_suns_nadir},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_poison"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {red_suns_nadir},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_slowness"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {green_suns_nadir},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"craft_cypher"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {craft_cypher},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"craft_trinket"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {craft_trinket},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"craft_artifact"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {craft_artifact},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"recharge_item"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {recharge_item},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"erase_item"}, .is_void = true, .is_member = false, .num_params = 0,
        .func_patterns = {erase_item},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"grow"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {overgrow},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"edify"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {edify_sapling},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"add_vel"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {impulse},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"teleport_forward"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {blink},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"play_note"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {make_note},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"fly_range"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {anchorites_flight},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"fly_duration"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {wayfarers_flight},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"change_color"}, .is_void = true, .is_member = false, .num_params = 0,
        .func_patterns = {internalize_pigment},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"change_shape"}, .is_void = true, .is_member = false, .num_params = 0,
        .func_patterns = {casters_glamour},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"place_block"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {place_block},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"destroy_liquid"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {destroy_liquid},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"destroy_fire"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {extinguish_area},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"destroy_sentinel"}, .is_void = true, .is_member = false, .num_params = 0,
        .func_patterns = {banish_sentinel},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_sentinel"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {summon_sentinel},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_block"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {conjure_block},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_fire"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {ignite},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_explosion"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {explosion},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_explosion_fire"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {fireball},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_light"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {conjure_light},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_water"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {create_water},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"craft_phial"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {craft_phial},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"flay_mind"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {flay_mind},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"weather_rain"}, .is_void = true, .is_member = false, .num_params = 0,
        .func_patterns = {summon_rain},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"weather_clear"}, .is_void = true, .is_member = false, .num_params = 0,
        .func_patterns = {dispel_rain},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"fly_wings"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {altiora},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"teleport_relative"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {greater_teleport},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"teleport_to"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {prospectors_gambit, compass_purification_II, subtractive_distillation, greater_teleport},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_regeneration"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {white_suns_zenith},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_night_vision"}, .is_void = true, .is_member = false, .num_params = 2,
        .func_patterns = {blue_suns_zenith},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_absorption"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {black_suns_zenith},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_haste"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {red_suns_zenith},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"effect_strength"}, .is_void = true, .is_member = false, .num_params = 3,
        .func_patterns = {green_suns_zenith},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_greater_sentinel"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {summon_greater_sentinel},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_lightning"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {summon_lightning},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"create_lava"}, .is_void = true, .is_member = false, .num_params = 1,
        .func_patterns = {create_lava},
        .eval = nullptr
        },
    // Non-Void Member
    InbuiltFunc {.names = {"pos"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {compass_purification_II},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"eye_pos"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {compass_purification},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"height"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {stadiometers_prfn},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"velocity"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {pace_purification},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"forward"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {alidades_purification},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"with", "with_back"}, .is_void = false, .is_member = true, .num_params = 1,
        .func_patterns = {integration_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);
            list.push_back(iotas[1]);
            return Iota{list};
        }
        },
    InbuiltFunc {.names = {"sublist"}, .is_void = false, .is_member = true, .num_params = 2,
        .func_patterns = {selection_exaltation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value) || !std::holds_alternative<double>(iotas[1].value) || !std::holds_alternative<double>(iotas[2].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);
            double ind1 = std::get<double>(iotas[1].value);
            double ind2 = std::get<double>(iotas[2].value);

            if (ind1 != (int)ind1 || ind2 != (int)ind2)
            {
                return Iota{.has_value = false};
            }

            return Iota{std::vector<Iota>(&list[(int)ind1], &list[(int)ind2])};
        }
        },
    InbuiltFunc {.names = {"back"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {derivation_decomposition, Pattern{bookkeepers_gambit, "v-"}},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);
            return Iota{list.back()};
        }
        },
    InbuiltFunc {.names = {"reversed"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {retrograde_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);
            return Iota{std::vector<Iota>(list.rbegin(), list.rend())};
        }
        },
    InbuiltFunc {.names = {"without_at"}, .is_void = false, .is_member = true, .num_params = 1,
        .func_patterns = {excisors_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value) || !std::holds_alternative<double>(iotas[1].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);
            double ind = std::get<double>(iotas[1].value);

            if (ind != (int)ind)
            {
                return Iota{.has_value = false};
            }

            list.erase(list.begin() + (int)ind);
            return Iota{list};
        }
        },
    InbuiltFunc {.names = {"with_front"}, .is_void = false, .is_member = true, .num_params = 1,
        .func_patterns = {speakers_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);
            list.insert(list.begin(), iotas[1]);
            return Iota{list};
        }
        },
    InbuiltFunc {.names = {"without_duplicates"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {uniqueness_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);
            std::vector<Iota> seen;
            std::vector<Iota> retVal;
            for (Iota i : list)
            {
                if (std::find(seen.begin(), seen.end(), i) == seen.end())
                {
                    retVal.push_back(i);
                    seen.push_back(i);
                }
            }

            return Iota{retVal};
        }
        },
    InbuiltFunc {.names = {"front"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {speakers_decomposition, Pattern{bookkeepers_gambit, "v-"}},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);

            if (list.size() < 1)
            {
                return Iota{.has_value = false};
            }

            return list[0];
        }
        },
    InbuiltFunc {.names = {"x"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {vector_disintegration, Pattern{bookkeepers_gambit, "vv"}},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);
            return Iota{vec[0]};
        }
        },
    InbuiltFunc {.names = {"y"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {vector_disintegration, Pattern{bookkeepers_gambit, "v-v"}},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);
            return Iota{vec[1]};
        }
        },
    InbuiltFunc {.names = {"z"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {vector_disintegration, Pattern{bookkeepers_gambit, "vv-"}},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);
            return Iota{vec[2]};
        }
        },
    InbuiltFunc {.names = {"sign"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {axial_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);

                double x = vec[0], y = vec[1], z = vec[2];

                double xn = std::abs(x);
                double yn = std::abs(y);
                double zn = std::abs(z);

                if (xn >= yn && xn >= zn) {
                    return Iota(x > 0 ? std::vector<double>{1, 0, 0} : std::vector<double>{-1, 0, 0});
                } else if (yn > xn && yn >= zn) {
                    return Iota(y > 0 ? std::vector<double>{0, 1, 0} : std::vector<double>{0, -1, 0});
                } else {
                    return Iota(z > 0 ? std::vector<double>{0, 0, 1} : std::vector<double>{0, 0, -1});
                }
            }
            else if (std::holds_alternative<double>(iotas[0].value))
            {
                double d = std::get<double>(iotas[0].value);

                if (d > 0)
                {
                    return Iota{1.0};
                }
                else if (d == 0)
                {
                    return Iota{0.0};
                }
                else
                {
                    return Iota{-1.0};
                }
            }
            else
            {
                return Iota{.has_value = false};
            }
        }
        },
    InbuiltFunc {.names = {"size", "length", "abs"}, .is_void = false, .is_member = true, .num_params = 0,
        .func_patterns = {length_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);

                return Iota(std::sqrt(std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0)));
            }
            else if (std::holds_alternative<double>(iotas[0].value))
            {
                double d = std::get<double>(iotas[0].value);

                return Iota(std::abs(d));
            }
            else if (std::holds_alternative<bool>(iotas[0].value))
            {
                bool b = std::get<bool>(iotas[0].value);

                return Iota(b ? 1.0 : 0.0);
            }
            else if (std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                std::vector<Iota> l = std::get<std::vector<Iota>>(iotas[0].value);

                return Iota((double)l.size());
            }
            else
            {
                return Iota{.has_value = false};
            }
        }
        },
    InbuiltFunc {.names = {"find"}, .is_void = false, .is_member = true, .num_params = 1,
        .func_patterns = {locators_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            std::vector<Iota> list = std::get<std::vector<Iota>>(iotas[0].value);

            double ind = std::find(list.begin(), list.end(), iotas[1]) - list.begin();

            if (ind >= list.size())
            {
                return Iota(-1.0);
            }

            return Iota(ind);
        }
        },
    // Non-Void Non-Member
    InbuiltFunc {.names = {"pow"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {power_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (std::holds_alternative<double>(iotas[0].value) && std::holds_alternative<double>(iotas[1].value))
            {
                double base = std::get<double>(iotas[0].value);
                double exp = std::get<double>(iotas[1].value);

                return Iota(std::pow(base, exp));
            }
            else if (std::holds_alternative<double>(iotas[0].value) && std::holds_alternative<std::vector<double>>(iotas[1].value))
            {
                double base = std::get<double>(iotas[0].value);
                std::vector<double> exp = std::get<std::vector<double>>(iotas[1].value);

                exp = {std::pow(base, exp[0]), std::pow(base, exp[1]), std::pow(base, exp[2])};

                return Iota(exp);
            }
            else if (std::holds_alternative<std::vector<double>>(iotas[0].value) && std::holds_alternative<double>(iotas[1].value))
            {
                std::vector<double> base = std::get<std::vector<double>>(iotas[0].value);
                double exp = std::get<double>(iotas[1].value);

                base = {std::pow(base[0], exp), std::pow(base[1], exp), std::pow(base[2], exp)};

                return Iota(base);
            }
            else if (std::holds_alternative<std::vector<double>>(iotas[0].value) && std::holds_alternative<std::vector<double>>(iotas[1].value))
            {
                // Project first vector onto second
                std::vector<double> from = std::get<std::vector<double>>(iotas[0].value);
                std::vector<double> onto = std::get<std::vector<double>>(iotas[1].value);

                double dotProd = std::inner_product(from.begin(), from.end(), onto.begin(), 0.0);
                double ontoLen = std::sqrt(std::inner_product(onto.begin(), onto.end(), onto.begin(), 0.0));
                double projLen = dotProd / ontoLen;

                std::vector<double> ontoNorm{ onto[0] / ontoLen, onto[1] / ontoLen, onto[2] / ontoLen };

                std::vector<double> proj{ ontoNorm[0] * projLen, ontoNorm[1] * projLen, ontoNorm[2] * projLen };

                return Iota(proj);
            }
            else
            {
                return Iota{.has_value = false};
            }
        }
        },
    InbuiltFunc {.names = {"floor"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {floor_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);

                vec = {std::floor(vec[0]), std::floor(vec[1]), std::floor(vec[2])};

                return Iota(vec);
            }
            else if (std::holds_alternative<double>(iotas[0].value))
            {
                double d = std::get<double>(iotas[0].value);

                return Iota(std::floor(d));
            }
            else
            {
                return Iota{.has_value = false};
            }
        }
        },
    InbuiltFunc {.names = {"ceil"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {ceiling_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);

                vec = {std::ceil(vec[0]), std::ceil(vec[1]), std::ceil(vec[2])};

                return Iota(vec);
            }
            else if (std::holds_alternative<double>(iotas[0].value))
            {
                double d = std::get<double>(iotas[0].value);

                return Iota(std::ceil(d));
            }
            else
            {
                return Iota{.has_value = false};
            }
        }
        },
    InbuiltFunc {.names = {"min"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {dioscuri_gambit, minimus_distillation, rotation_gambit_II, augurs_exaltation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value) || !std::holds_alternative<double>(iotas[1].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);
            double d2 = std::get<double>(iotas[1].value);

            return Iota(std::min(d1, d2));
        }
        },
    InbuiltFunc {.names = {"max"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {dioscuri_gambit, maximus_distillation, rotation_gambit_II, augurs_exaltation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value) || !std::holds_alternative<double>(iotas[1].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);
            double d2 = std::get<double>(iotas[1].value);

            return Iota(std::max(d1, d2));
        }
        },
    InbuiltFunc {.names = {"as_bool"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {augurs_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (std::holds_alternative<std::vector<double>>(iotas[0].value))
            {
                std::vector<double> vec = std::get<std::vector<double>>(iotas[0].value);

                return Iota(vec[0] != 0 || vec[1] != 0 || vec[2] != 0);
            }
            else if (std::holds_alternative<double>(iotas[0].value))
            {
                double d = std::get<double>(iotas[0].value);

                return Iota(d != 0);
            }
            else if (std::holds_alternative<void*>(iotas[0].value))
            {
                return Iota(false);
            }
            else if (std::holds_alternative<bool>(iotas[0].value))
            {
                return iotas[0];
            }
            if (std::holds_alternative<std::vector<Iota>>(iotas[0].value))
            {
                std::vector<Iota> l = std::get<std::vector<Iota>>(iotas[0].value);

                return Iota(l.size() > 0);
            }
            else
            {
                return Iota(true);
            }
        }
        },
    InbuiltFunc {.names = {"random"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {entropy_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"random"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {prospectors_gambit, subtractive_distillation, entropy_reflection, multiplicative_distillation, additive_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"tau"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {circle_reflection},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(M_PI * 2.0);
        }
        },
    InbuiltFunc {.names = {"pi"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {arcs_reflection},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(M_PI);
        }
        },
    InbuiltFunc {.names = {"e"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {eulers_reflection},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(M_E);
        }
        },
    InbuiltFunc {.names = {"sin"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {sine_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);

            return Iota(std::sin(d1));
        }
        },
    InbuiltFunc {.names = {"cos"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {cosine_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);

            return Iota(std::cos(d1));
        }
        },
    InbuiltFunc {.names = {"tan"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {tangent_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);

            return Iota(std::tan(d1));
        }
        },
    InbuiltFunc {.names = {"arc_sin"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {inverse_sine_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);

            return Iota(std::asin(d1));
        }
        },
    InbuiltFunc {.names = {"arc_cos"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {inverse_cosine_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);

            return Iota(std::acos(d1));
        }
        },
    InbuiltFunc {.names = {"arc_tan"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {inverse_tangent_purification},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);

            return Iota(std::atan(d1));
        }
        },
    InbuiltFunc {.names = {"angle"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {inverse_tangent_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value) || !std::holds_alternative<double>(iotas[1].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);
            double d2 = std::get<double>(iotas[1].value);

            return Iota(std::atan2(d1, d2));
        }
        },
    InbuiltFunc {.names = {"log"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {logarithmic_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value) || !std::holds_alternative<double>(iotas[1].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);
            double d2 = std::get<double>(iotas[1].value);

            return Iota(std::log(d1) / std::log(d2));
        }
        },
    InbuiltFunc {.names = {"ln"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {eulers_reflection, logarithmic_distillation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);

            return Iota(std::log(d1));
        }
        },
    InbuiltFunc {.names = {"vec"}, .is_void = false, .is_member = false, .num_params = 3,
        .func_patterns = {vector_exaltation},
        .eval = [](std::vector<Iota> iotas) {
            if (!std::holds_alternative<double>(iotas[0].value) || !std::holds_alternative<double>(iotas[1].value) || !std::holds_alternative<double>(iotas[2].value))
            {
                return Iota{.has_value = false};
            }

            double d1 = std::get<double>(iotas[0].value);
            double d2 = std::get<double>(iotas[1].value);
            double d3 = std::get<double>(iotas[2].value);

            return Iota(std::vector<double>{d1, d2, d3});
        }
        },
    InbuiltFunc {.names = {"vec0"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {vector_reflection_zero},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(std::vector<double>{0.0, 0.0, 0.0});
        }
        },
    InbuiltFunc {.names = {"vecXP"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {vector_reflection_PX},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(std::vector<double>{1.0, 0.0, 0.0});
        }
        },
    InbuiltFunc {.names = {"vecXN"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {vector_reflection_NX},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(std::vector<double>{-1.0, 0.0, 0.0});
        }
        },
    InbuiltFunc {.names = {"vecYP", "vec_up"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {vector_reflection_PY},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(std::vector<double>{0.0, 1.0, 0.0});
        }
        },
    InbuiltFunc {.names = {"vecYN", "vec_down"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {vector_reflection_NY},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(std::vector<double>{0.0, -1.0, 0.0});
        }
        },
    InbuiltFunc {.names = {"vecZP"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {vector_reflection_PZ},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(std::vector<double>{0.0, 0.0, 1.0});
        }
        },
    InbuiltFunc {.names = {"vecZN"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {vector_reflection_NZ},
        .eval = [](std::vector<Iota> iotas) {
            return Iota(std::vector<double>{0.0, 0.0, -1.0});
        }
        },
    InbuiltFunc {.names = {"sentinel_pos"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {locate_sentinel},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"sentinel_dir_from"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {wayfind_sentinel},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"is_flying"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {aviators_purification},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"self"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {minds_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"circle_impetus_pos"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {waystone_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"circle_impetus_forward"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {lodestone_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"circle_LNW"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {lesser_fold_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"circle_USE"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {greater_fold_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"block_raycast"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {gemini_decomposition, compass_purification, jesters_gambit, alidades_purification, archers_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"block_raycast"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {archers_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"block_normal_raycast"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {gemini_decomposition, compass_purification, jesters_gambit, alidades_purification, architects_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"block_normal_raycast"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {architects_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"entity_raycast"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {gemini_decomposition, compass_purification, jesters_gambit, alidades_purification, scouts_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"entity_raycast"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {scouts_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_entity"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {entity_prfn},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_entities"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_any},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_animal"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {entity_prfn_animal},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_animals"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_animal},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_monster"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {entity_prfn_monster},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_monsters"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_monster},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_item"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {entity_prfn_item},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_items"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_item},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_player"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {entity_prfn_player},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_players"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_player},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_living"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {entity_prfn_living},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_living"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_living},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_non_animals"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_non_animal},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_non_monsters"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_non_monster},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_non_items"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_non_item},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_non_players"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_non_player},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"get_non_living"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {zone_dstl_non_living},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"read"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {scribes_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"read"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {chroniclers_prfn},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"can_read"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {auditors_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"can_read"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {auditors_purification},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"can_write"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {assessors_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"can_write"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {assessors_purification},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"read_akashic"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {akashas_distillation},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {singles_purification, muninns_reflection, nullary_reflection, huginns_gambit, introspection, flocks_reflection, flocks_gambit, derivation_decomposition,
            Pattern{bookkeepers_gambit, "v-"}, hermes_gambit, retrospection, rotation_gambit, thoths_gambit, jesters_gambit, huginns_gambit },
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {Pattern{numerical_reflection, "2"}, flocks_gambit, singles_purification, muninns_reflection, nullary_reflection, huginns_gambit, introspection,
            flocks_reflection, flocks_gambit, derivation_decomposition, Pattern{bookkeepers_gambit, "v-"}, flocks_disintegration, jesters_gambit, hermes_gambit, retrospection,
            rotation_gambit, thoths_gambit, jesters_gambit, huginns_gambit },
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute_no_ravens_mind"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {singles_purification, introspection, flocks_reflection, flocks_gambit, derivation_decomposition, Pattern{bookkeepers_gambit, "v-"}, hermes_gambit,
            retrospection, jesters_gambit, thoths_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute_no_ravens_mind"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {Pattern{numerical_reflection, "2"}, flocks_gambit, singles_purification, introspection, flocks_reflection, flocks_gambit, derivation_decomposition,
            Pattern{bookkeepers_gambit, "v-"}, flocks_disintegration, jesters_gambit, hermes_gambit, retrospection, jesters_gambit, thoths_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute_unsafe"}, .is_void = false, .is_member = false, .num_params = 1,
        .func_patterns = {hermes_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"execute_unsafe"}, .is_void = false, .is_member = false, .num_params = 2,
        .func_patterns = {jesters_gambit, hermes_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"patterns_remaining"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {thanatos_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"stack_size"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {flocks_reflection},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"dump_stack"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {introspection, Pattern{bookkeepers_gambit, "v"}, retrospection, nullary_reflection, singles_purification, thoths_gambit},
        .eval = nullptr
        },
    InbuiltFunc {.names = {"dump_ravens_mind"}, .is_void = false, .is_member = false, .num_params = 0,
        .func_patterns = {muninns_reflection},
        .eval = nullptr
        },
};