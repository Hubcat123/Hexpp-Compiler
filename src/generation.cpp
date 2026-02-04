#include "generation.hpp"

#include <sstream>

#include "util.hpp"

Generator::Generator(const NodeProg* prog)
    :m_prog(prog)
{ }

std::vector<Pattern> Generator::generate()
{
    gen_prog();

    return m_output;
}

void Generator::gen_assignment(const NodeTermVar* term_var, const std::variant<const NodeExpr*, const float> value, TokenType_ op, size_t line, bool is_post)
{
    // Add value to top of stack
    struct ValueVisitor {
        Generator& gen;
        ValueVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeExpr* expr)
        {
            gen.gen_expr(expr);
        }

        void operator()(const float num)
        {
            // Cut off decimal part if it's not used
            if (num == (int)num)
            {
                gen.numerical_reflection(std::to_string((int)num));
            }
            else
            {
                gen.numerical_reflection(std::to_string(num));
            }
        }
    };

    ValueVisitor visitor(*this);
    std::visit(visitor, value);

    // Extract var info
    struct VarVisitor {
        Generator& gen;
        bool& is_subscript;
        std::string& ident_name;
        NodeExpr*& subscript_expr;
        VarVisitor (Generator& _gen, bool& _is_subscript, std::string& _ident_name, NodeExpr*& _subscript_expr)
            :gen(_gen), is_subscript(_is_subscript), ident_name(_ident_name), subscript_expr(_subscript_expr) {}
        
        void operator()(const NodeVarIdent* ident)
        {
            is_subscript = false;
            ident_name = ident->ident.value.value();
        }

        void operator()(const NodeVarListSubscript* list)
        {
            is_subscript = true;
            ident_name = list->ident.value.value();
            subscript_expr = list->expr;
        }
    };

    bool is_subscript;
    std::string ident_name;
    NodeExpr* subscript_expr = nullptr;

    VarVisitor varVisitor(*this, is_subscript, ident_name, subscript_expr);
    std::visit(varVisitor, term_var->var);

    Var var = gen_var_ident(ident_name, term_var->line, op == TokenType_::eq && !is_subscript, false);

    // Gen index and handle list
    if (is_subscript)
    {
        gen_expr(subscript_expr);

        // If it's not eq, grab relavent list item
        if (op != TokenType_::eq)
        {
            // Duplicate list and index so we can re-assign it later
            dioscuri_gambit();
            selection_distillation();
            numerical_reflection("3");
            fishermans_gambit();
            jesters_gambit();
        }
        else
        {
            rotation_gambit();
        }
    }

    // Make duplicate pre-op if it's a post-op
    if (is_post)
    {
        gemini_decomposition();
    }

    switch (op)
    {
    case TokenType_::eq:
        // Remove prev value from stack if local. Global var will be overwritten later
        if (!var.is_global && !is_subscript)
        {
            pop();
        }
        break;
    case TokenType_::plus_eq:
        additive_distillation();
        break;
    case TokenType_::dash_eq:
        jesters_gambit();
        subtractive_distillation();
        break;
    case TokenType_::star_eq:
        multiplicative_distillation();
        break;
    case TokenType_::fslash_eq:
        jesters_gambit();
        division_distillation();
        break;
    case TokenType_::mod_eq:
        jesters_gambit();
        modulus_distillation();
        break;
    }

    // Place value in list
    if (is_subscript)
    {
        surgeons_exaltation();
    }
    
    // If local
    if (!var.is_global)
    {
        numerical_reflection(std::to_string(-(int)m_stack_size + (int)var.stack_loc + 1));

        // Don't duplicate down if post-op
        if (is_post)
        {
            fishermans_gambit();
        }
        else
        {
            fishermans_gambit_II();
        }
    }
    // If global
    else
    {
        // Don't duplicate if post-op
        if (!is_post)
        {
            gemini_decomposition();
        }

        muninns_reflection();
        numerical_reflection(std::to_string(var.stack_loc));
        rotation_gambit();
        surgeons_exaltation();
        huginns_gambit();
    }
}

bool Generator::gen_inbuilt_func(const NodeDefinedFunc* func, bool is_void, bool is_member)
{
    std::string func_name = func->ident.value.value();

    if (is_void)
    {
        if (is_member)
        { }
        else
        {
            if (func_name == "write") {
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    scribes_gambit();
                }
                else
                {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    chroniclers_gambit();
                }
                return true;
            } else if (func_name == "write_akashic") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                akashas_gambit();
                return true;
            } else if (func_name == "print") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                reveal();
                pop();
                return true;
            } else if (func_name == "execute_unsafe_no_ret") {
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    add_pattern(PatternType::hermes_gambit, -1);
                }
                else
                {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    jesters_gambit();
                    add_pattern(PatternType::hermes_gambit, -2);
                }
                return true;
            } else if (func_name == "mine") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                break_block();
                return true;
            } else if (func_name == "effect_weakness") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                white_suns_nadir();
                return true;
            } else if (func_name == "effect_levitation") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                blue_suns_nadir();
                return true;
            } else if (func_name == "effect_withering") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                black_suns_nadir();
                return true;
            } else if (func_name == "effect_poison") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                red_suns_nadir();
                return true;
            } else if (func_name == "effect_slowness") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                green_suns_nadir();
                return true;
            } else if (func_name == "craft_cypher") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                craft_cypher();
                return true;
            } else if (func_name == "craft_trinket") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                craft_trinket();
                return true;
            } else if (func_name == "craft_artifact") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                craft_artifact();
                return true;
            } else if (func_name == "recharge_item") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                recharge_item();
                return true;
            } else if (func_name == "erase_item") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                erase_item();
                return true;
            } else if (func_name == "grow") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                overgrow();
                return true;
            } else if (func_name == "edify") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                edify_sapling();
                return true;
            } else if (func_name == "add_vel") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                impulse();
                return true;
            } else if (func_name == "teleport_forward") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                blink();
                return true;
            } else if (func_name == "play_note") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                make_note();
                return true;
            } else if (func_name == "fly_range") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                anchorites_flight();
                return true;
            } else if (func_name == "fly_duration") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                wayfarers_flight();
                return true;
            } else if (func_name == "change_color") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                internalize_pigment();
                return true;
            } else if (func_name == "change_shape") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                casters_glamour();
                return true;
            } else if (func_name == "place_block") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                place_block();
                return true;
            } else if (func_name == "destroy_liquid") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                destroy_liquid();
                return true;
            } else if (func_name == "destroy_fire") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                extinguish_area();
                return true;
            } else if (func_name == "destroy_sentinel") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                banish_sentinel();
                return true;
            } else if (func_name == "create_sentinel") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                summon_sentinel();
                return true;
            } else if (func_name == "create_block") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                conjure_block();
                return true;
            } else if (func_name == "create_fire") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                ignite();
                return true;
            } else if (func_name == "create_explosion") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                explosion();
                return true;
            } else if (func_name == "create_explosion_fire") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                fireball();
                return true;
            } else if (func_name == "create_light") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                conjure_light();
                return true;
            } else if (func_name == "create_water") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                create_water();
                return true;
            // Great Spells
            } else if (func_name == "craft_phial") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                craft_phial();
                return true;
            } else if (func_name == "flay_mind") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                flay_mind();
                return true;
            } else if (func_name == "weather_rain") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                summon_rain();
                return true;
            } else if (func_name == "weather_clear") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                dispel_rain();
                return true;
            } else if (func_name == "fly_wings") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                altiora();
                return true;
            } else if (func_name == "teleport_relative") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                greater_teleport();
                return true;
            } else if (func_name == "teleport_to") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                prospectors_gambit();
                compass_purification_II();
                subtractive_distillation();
                greater_teleport();
                return true;
            } else if (func_name == "effect_regeneration") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                white_suns_zenith();
                return true;
            } else if (func_name == "effect_night_vision") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                blue_suns_zenith();
                return true;
            } else if (func_name == "effect_absorption") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                black_suns_zenith();
                return true;
            } else if (func_name == "effect_haste") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                red_suns_zenith();
                return true;
            } else if (func_name == "effect_strength") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                green_suns_zenith();
                return true;
            } else if (func_name == "create_greater_sentinel") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                summon_greater_sentinel();
                return true;
            } else if (func_name == "create_lightning") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                summon_lightning();
                return true;
            } else if (func_name == "create_lava") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                create_lava();
                return true;
            }
        }
    }
    else
    {
        if (is_member)
        {
            if (func_name == "pos") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                compass_purification_II();
                return true;
            } else if (func_name == "eye_pos") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                compass_purification();
                return true;
            } else if (func_name == "height") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                stadiometers_purification();
                return true;
            } else if (func_name == "velocity") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                pace_purification();
                return true;
            } else if (func_name == "forward") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                alidades_purification();
                return true;
            } else if (func_name == "with" || func_name == "with_back") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                integration_distillation();
                return true;
            } else if (func_name == "sublist") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                selection_exaltation();
                return true;
            } else if (func_name == "back") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                derivation_decomposition();
                add_pattern(PatternType::bookkeepers_gambit, -1, "v-");
                return true;
            } else if (func_name == "reversed") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                retrograde_purification();
                return true;
            } else if (func_name == "without_at") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                excisors_distillation();
                return true;
            } else if (func_name == "with_front") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                speakers_distillation();
                return true;
            } else if (func_name == "without_duplicates") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                uniqueness_purification();
                return true;
            } else if (func_name == "front") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                speakers_decomposition();
                add_pattern(PatternType::bookkeepers_gambit, -1, "v-");
                return true;
            } else if (func_name == "x") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_disintegration();
                pop(2);
                return true;
            } else if (func_name == "y") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_disintegration();
                add_pattern(PatternType::bookkeepers_gambit, -2, "v-v");
                return true;
            } else if (func_name == "z") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_disintegration();
                add_pattern(PatternType::bookkeepers_gambit, -2, "vv-");
                return true;
            } else if (func_name == "sign") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                axial_purification();
                return true;
            } else if (func_name == "size" || func_name == "length" || func_name == "abs") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                length_purification();
                return true;
            } else if (func_name == "find") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                locators_distillation();
                return true;
            }
        }
        else
        {
            if (func_name == "pow") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                power_distillation();
                return true;
            } else if (func_name == "floor") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                floor_purification();
                return true;
            } else if (func_name == "ceil") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                ceiling_purification();
                return true;
            } else if (func_name == "min") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                dioscuri_gambit();
                minimus_distillation();
                rotation_gambit_II();
                augurs_exaltation();
                return true;
            } else if (func_name == "max") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                dioscuri_gambit();
                maximus_distillation();
                rotation_gambit_II();
                augurs_exaltation();
                return true;
            } else if (func_name == "as_bool") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                augurs_purification();
                return true;
            } else if (func_name == "random") {
                if (func->exprs.size() == 0) {
                    entropy_reflection();
                } else {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    prospectors_gambit();
                    subtractive_distillation();
                    entropy_reflection();
                    multiplicative_distillation();
                    additive_distillation();
                }
                return true;
            } else if (func_name == "tau") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                circle_reflection();
                return true;
            } else if (func_name == "pi") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                arcs_reflection();
                return true;
            } else if (func_name == "e") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                eulers_reflection();
                return true;
            } else if (func_name == "sin") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                sine_purification();
                return true;
            } else if (func_name == "cos") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                cosine_purification();
                return true;
            } else if (func_name == "tan") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                tangent_purification();
                return true;
            } else if (func_name == "arc_sin") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                inverse_sine_purification();
                return true;
            } else if (func_name == "arc_cos") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                inverse_cosine_purification();
                return true;
            } else if (func_name == "arc_tan") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                inverse_tangent_purification();
                return true;
            } else if (func_name == "angle") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                inverse_tangent_distillation();
                return true;
            } else if (func_name == "log") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                logarithmic_distillation();
                return true;
            } else if (func_name == "ln") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                eulers_reflection();
                logarithmic_distillation();
                return true;
            } else if (func_name == "vec") {
                try_gen_x_exprs(func->exprs, 3, func->line);
                vector_exaltation();
                return true;
            } else if (func_name == "vec0") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_reflection_zero();
                return true;
            } else if (func_name == "vecXP") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_reflection_PX();
                return true;
            } else if (func_name == "vecXN") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_reflection_NX();
                return true;
            } else if (func_name == "vecYP" || func_name == "vec_up") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_reflection_PY();
                return true;
            } else if (func_name == "vecYN" || func_name == "vec_down") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_reflection_NY();
                return true;
            } else if (func_name == "vecZP") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_reflection_PZ();
                return true;
            } else if (func_name == "vecZN") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                vector_reflection_NZ();
                return true;
            } else if (func_name == "sentinel_pos") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                locate_sentinel();
                return true;
            } else if (func_name == "sentinel_dir_from") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                wayfind_sentinel();
                return true;
            } else if (func_name == "is_flying") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                aviators_purification();
                return true;
            } else if (func_name == "self") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                minds_reflection();
                return true;
            } else if (func_name == "circle_impetus_pos") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                waystone_reflection();
                return true;
            } else if (func_name == "circle_impetus_forward") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                lodestone_reflection();
                return true;
            } else if (func_name == "circle_LNW") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                lesser_fold_reflection();
                return true;
            } else if (func_name == "circle_USE") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                greater_fold_reflection();
                return true;
            } else if (func_name == "block_raycast") {
                // Raycast from an entity
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    gemini_decomposition();
                    compass_purification();
                    jesters_gambit();
                    alidades_purification();
                    archers_distillation();
                } else {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    archers_distillation();
                }
                return true;
            } else if (func_name == "block_normal_raycast") {
                // Raycast from an entity
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    gemini_decomposition();
                    compass_purification();
                    jesters_gambit();
                    alidades_purification();
                    architects_distillation();
                } else {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    architects_distillation();
                }
                return true;
            } else if (func_name == "entity_raycast") {
                // Raycast from an entity
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    gemini_decomposition();
                    compass_purification();
                    jesters_gambit();
                    alidades_purification();
                    scouts_distillation();
                } else {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    scouts_distillation();
                }
                return true;
            } else if (func_name == "get_entity") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                entity_prfn();
                return true;
            } else if (func_name == "get_entities") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_any();
                return true;
            } else if (func_name == "get_animal") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                entity_prfn_animal();
                return true;
            } else if (func_name == "get_animals") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_animal();
                return true;
            } else if (func_name == "get_monster") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                entity_prfn_monster();
                return true;
            } else if (func_name == "get_monsters") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_monster();
                return true;
            } else if (func_name == "get_item") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                entity_prfn_item();
                return true;
            } else if (func_name == "get_items") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_item();
                return true;
            } else if (func_name == "get_player") {
                try_gen_x_exprs(func->exprs, 1, func->line);
                entity_prfn_player();
                return true;
            } else if (func_name == "get_players") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_player();
                return true;
            } else if (func_name == "get_living") {
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    entity_prfn_living();
                } else {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    zone_dstl_living();
                }
                return true;
            } else if (func_name == "get_non_animals") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_non_animal();
                return true;
            } else if (func_name == "get_non_monsters") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_non_monster();
                return true;
            } else if (func_name == "get_non_items") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_non_item();
                return true;
            } else if (func_name == "get_non_players") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_non_player();
                return true;
            } else if (func_name == "get_non_living") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                zone_dstl_non_living();
                return true;
            } else if (func_name == "read") {
                if (func->exprs.size() == 0) {
                    scribes_reflection();
                }
                else
                {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    chroniclers_purification();
                }
                return true;
            } else if (func_name == "can_read") {
                if (func->exprs.size() == 0) {
                    auditors_reflection();
                }
                else
                {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    auditors_purification();
                }
                return true;
            } else if (func_name == "can_write") {
                if (func->exprs.size() == 0) {
                    assessors_reflection();
                }
                else
                {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    assessors_purification();
                }
                return true;
            } else if (func_name == "read_akashic") {
                try_gen_x_exprs(func->exprs, 2, func->line);
                akashas_distillation();
                return true;
            } else if (func_name == "execute") {
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    singles_purification();
                    muninns_reflection();
                    nullary_reflection();
                    huginns_gambit();
                    add_pattern(PatternType::introspection, 0);
                    flocks_reflection();
                    add_pattern(PatternType::flocks_gambit, 0);
                    derivation_decomposition();
                    bookkeepers_gambit("v-");
                    add_pattern(PatternType::hermes_gambit, 0);
                    add_pattern(PatternType::retrospection, 0);
                    rotation_gambit();
                    add_pattern(PatternType::thoths_gambit, 0);
                    jesters_gambit();
                    huginns_gambit();
                    // Account properly for stack size
                    --m_stack_size;
                }
                else
                {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    numerical_reflection("2");
                    flocks_gambit(2);
                    singles_purification();
                    muninns_reflection();
                    nullary_reflection();
                    huginns_gambit();
                    add_pattern(PatternType::introspection, 0);
                    flocks_reflection();
                    add_pattern(PatternType::flocks_gambit, 0);
                    derivation_decomposition();
                    bookkeepers_gambit("v-");
                    add_pattern(PatternType::flocks_disintegration, 0);
                    jesters_gambit();
                    add_pattern(PatternType::hermes_gambit, 0);
                    add_pattern(PatternType::retrospection, 0);
                    rotation_gambit();
                    add_pattern(PatternType::thoths_gambit, 0);
                    jesters_gambit();
                    huginns_gambit();
                    // Account properly for stack size
                    --m_stack_size;
                }
                return true;
            } else if (func_name == "execute_no_ravens_mind") {
                if (func->exprs.size() == 1) {
                    add_pattern(PatternType::introspection, 0);
                    flocks_reflection();
                    add_pattern(PatternType::flocks_gambit, 0);
                    derivation_decomposition();
                    bookkeepers_gambit("v-");
                    add_pattern(PatternType::hermes_gambit, 0);
                    add_pattern(PatternType::retrospection, 0);
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    singles_purification();
                    add_pattern(PatternType::thoths_gambit, 0);
                    // Account properly for stack size
                    --m_stack_size;
                }
                else
                {
                    add_pattern(PatternType::introspection, 0);
                    flocks_reflection();
                    add_pattern(PatternType::flocks_gambit, 0);
                    derivation_decomposition();
                    bookkeepers_gambit("v-");
                    add_pattern(PatternType::flocks_disintegration, 0);
                    jesters_gambit();
                    add_pattern(PatternType::hermes_gambit, 0);
                    add_pattern(PatternType::retrospection, 0);
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    numerical_reflection("2");
                    flocks_gambit(2);
                    singles_purification();
                    add_pattern(PatternType::thoths_gambit, 0);
                    // Account properly for stack size
                    --m_stack_size;
                }
                return true;
            } else if (func_name == "execute_unsafe") {
                if (func->exprs.size() == 1) {
                    try_gen_x_exprs(func->exprs, 1, func->line);
                    add_pattern(PatternType::hermes_gambit, 0);
                }
                else
                {
                    try_gen_x_exprs(func->exprs, 2, func->line);
                    jesters_gambit();
                    add_pattern(PatternType::hermes_gambit, -1);
                }
                return true;
            } else if (func_name == "patterns_remaining") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                thanatos_reflection();
                return true;
            } else if (func_name == "stack_size") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                flocks_reflection();
                return true;
            } else if (func_name == "dump_stack") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                add_pattern(PatternType::introspection, 0);
                pop();
                flocks_reflection();
                add_pattern(PatternType::flocks_gambit, 0);
                add_pattern(PatternType::retrospection, 0);
                numerical_reflection("0");
                singles_purification();
                add_pattern(PatternType::thoths_gambit, 0);
                add_pattern(PatternType::flocks_disintegration, 0);
                return true;
            } else if (func_name == "dump_ravens_mind") {
                try_gen_x_exprs(func->exprs, 0, func->line);
                muninns_reflection();
                return true;
            }
        }
    }

    return false;
}

bool Generator::gen_call_func(const NodeDefinedFunc* func)
{
    // Find function being called
    std::vector<Func>::iterator iter = std::find_if(m_funcs.begin(), m_funcs.end(), [&](const Func& _func){
        return _func.name == func->ident.value.value() && _func.num_params == func->exprs.size();});
    if (iter == m_funcs.end())
    {
        compilation_error(std::string("No function defined with this name with the passed number of parameters: ") + func->ident.value.value(), func->line);
    }

    // Generate expressions
    for (NodeExpr* expr : func->exprs)
    {
        gen_expr(expr);
    }

    // Execute function code
    muninns_reflection();
    numerical_reflection(std::to_string(iter->stack_loc));
    selection_distillation();
    // Iris' Gambit
    add_pattern(PatternType::iris_gambit, 0);

    // Account for function cleaning up exprs
    m_stack_size -= func->exprs.size();

    // Account for expression left on stack from non-void function and function iota being consumed
    if (iter->is_void)
    {
        --m_stack_size;
    }

    return iter->is_void;
}

void Generator::gen_bin_expr(const NodeExprBin* expr_bin)
{
    // If binary expression is a type of assignment
    if (expr_bin->op_type == TokenType_::eq || expr_bin->op_type == TokenType_::plus_eq || expr_bin->op_type == TokenType_::dash_eq || expr_bin->op_type == TokenType_::star_eq
         || expr_bin->op_type == TokenType_::fslash_eq || expr_bin->op_type == TokenType_::mod_eq)
    {
        if (!std::holds_alternative<NodeTerm*>(expr_bin->lhs->var))
        {
            compilation_error("Expected identifier", expr_bin->line);
        }

        NodeTerm* term = std::get<NodeTerm*>(expr_bin->lhs->var);
        if (!std::holds_alternative<NodeTermVar*>(term->var))
        {
            compilation_error("Expected identifier", expr_bin->line);
        }

        gen_assignment(std::get<NodeTermVar*>(term->var), expr_bin->rhs, expr_bin->op_type, expr_bin->line);

        return;
    }

    gen_expr(expr_bin->lhs);

    // If binary expression is calling a member function
    if (expr_bin->op_type == TokenType_::dot)
    {
        // If rhs is a term
        if (std::holds_alternative<NodeTerm*>(expr_bin->rhs->var))
        {
            NodeTerm* term = std::get<NodeTerm*>(expr_bin->rhs->var);
            // If term is a function
            if (std::holds_alternative<NodeTermCallFunc*>(term->var))
            {
                NodeTermCallFunc* term_func = std::get<NodeTermCallFunc*>(term->var);

                if (!gen_inbuilt_func(term_func->func, false, true))
                {
                    compilation_error("Expected member function", expr_bin->line);
                }
                return;
            }
            else
            {
                compilation_error("Expected member function", expr_bin->line);
            }
        }
        else
        {
            compilation_error("Expected member function", expr_bin->line);
        }
    }

    gen_expr(expr_bin->rhs);

    switch(expr_bin->op_type)
    {
    case TokenType_::double_eq:
        equality_distillation();
        break;
    case TokenType_::not_eq_:
        inequality_distillation();
        break;
    case TokenType_::angle_open:
        minimus_distillation();
        break;
    case TokenType_::oangle_eq:
        minimus_distillation_II();
        break;
    case TokenType_::angle_close:
        maximus_distillation();
        break;
    case TokenType_::cangle_eq:
        maximus_distillation_II();
        break;
    case TokenType_::plus:
        additive_distillation();
        break;
    case TokenType_::dash:
        subtractive_distillation(); 
        break;
    case TokenType_::star:
        multiplicative_distillation();
        break;
    case TokenType_::slash_forward:
        division_distillation();
        break;
    case TokenType_::modulus:
        modulus_distillation();
        break;
    case TokenType_::double_amp:
        conjunction_distillation();
        break;
    case TokenType_::double_bar:
        disjunction_distillation();
        break;
    case TokenType_::caret:
        exclusion_distillation();
        break;
    }
}

Generator::Var Generator::gen_var_ident(const std::string ident_name, size_t line, bool dont_gen_if_global, bool leave_copy)
{
    std::vector<Var>::iterator iter = std::find_if(m_vars.begin(), m_vars.end(),
        [&](const Var& var){ return var.name == ident_name; });
    
    if (iter == m_vars.end())
    {
        iter = std::find_if(m_global_vars.begin(), m_global_vars.end(),
            [&](const Var& var){ return var.name == ident_name; });
        
        if (iter == m_global_vars.end())
        {
            compilation_error(std::string("Undeclared identifier: ") + ident_name, line);
        }
    }

    Var& var = *iter;

    if (var.is_global)
    {
        if (!dont_gen_if_global)
        {
            muninns_reflection();
            numerical_reflection(std::to_string(var.stack_loc));
            selection_distillation();
        }
    }
    else
    {
        numerical_reflection(std::to_string((int)m_stack_size - 1 - (int)var.stack_loc));

        if (leave_copy)
        {
            fishermans_gambit_II();
        }
        else
        {
            fishermans_gambit();
        }
    }

    return var;
}

void Generator::gen_var(const NodeTermVar* var)
{
    struct VarVisitor {
        Generator& gen;
        VarVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeVarIdent* var_ident)
        {
            gen.gen_var_ident(var_ident->ident.value.value(), var_ident->line);
        }

        void operator()(const NodeVarListSubscript* var_list)
        {
            gen.gen_var_ident(var_list->ident.value.value(), var_list->line);
            gen.gen_expr(var_list->expr);
            gen.selection_distillation();
        }
    };

    VarVisitor visitor(*this);
    std::visit(visitor, var->var);
}

void Generator::gen_term(const NodeTerm* term)
{
    struct TermVisitor {
        Generator& gen;
        TermVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeTermUn* term_un)
        {
            // If unary expression is a type of assignment
            if (term_un->op_type == TokenType_::double_plus || term_un->op_type == TokenType_::double_dash)
            {
                // Check to make sure term is a var
                if (!std::holds_alternative<NodeTermVar*>(term_un->term->var))
                {
                    compilation_error("Expected identifier", term_un->line);
                }

                gen.gen_assignment(std::get<NodeTermVar*>(term_un->term->var), (float)((term_un->op_type == TokenType_::double_plus) ? 1 : -1), TokenType_::plus_eq, term_un->line);

                return;
            }

            gen.gen_term(term_un->term);

            switch (term_un->op_type)
            {
            case TokenType_::dash:
                gen.numerical_reflection("-1");
                gen.multiplicative_distillation();
                break;
            case TokenType_::tilde:
            case TokenType_::not_:
                gen.negation_purification();
                break;
            }
        }

        void operator()(const NodeTermUnPost* term_un_post)
        {
            gen.gen_assignment(term_un_post->vari, (float)((term_un_post->op_type == TokenType_::double_plus) ? 1 : -1), TokenType_::plus_eq, term_un_post->line, true);
        }

        void operator()(const NodeTermNumLit* term_int_lit)
        {
            gen.numerical_reflection(term_int_lit->num_lit.value.value());
        }

        void operator()(const NodeTermListLit* term_list_lit)
        {
            if (term_list_lit->exprs.size() <= 0)
            {
                gen.vacant_reflection();
            }
            else
            {
                for (NodeExpr* expr : term_list_lit->exprs)
                {
                    gen.gen_expr(expr);
                }

                gen.numerical_reflection(std::to_string(term_list_lit->exprs.size()));
                gen.flocks_gambit(term_list_lit->exprs.size());
            }
        }

        void operator()(const NodeTermPatternLit* term_pattern_lit)
        {
            gen.add_pattern(PatternType::introspection, 0);
            gen.add_pattern(PatternType::pattern_lit, 0, term_pattern_lit->pattern_lit.value.value());
            gen.add_pattern(PatternType::retrospection, 1);
            gen.add_pattern(PatternType::flocks_disintegration, 0);
        }

        void operator()(const NodeTermBoolLit* term_bool_lit)
        {
            if (term_bool_lit->bool_.value == "true")
            {
                gen.true_reflection();
            }
            else
            {
                gen.false_reflection();
            }
        }

        void operator()(const NodeTermNullLit* term_null_lit)
        {
            gen.nullary_reflection();
        }

        void operator()(const NodeTermVar* term_var)
        {
            gen.gen_var(term_var);
        }

        void operator()(const NodeTermParen* term_paren)
        {
            gen.gen_expr(term_paren->expr);
        }

        void operator()(const NodeTermCallFunc* call_func)
        {
            if (!gen.gen_inbuilt_func(call_func->func, false, false) && gen.gen_call_func(call_func->func))
            {
                compilation_error("Calling void function as non-void function", call_func->line);
            }
        }
    };

    TermVisitor visitor(*this);
    std::visit(visitor, term->var);
}

void Generator::gen_expr(const NodeExpr* expr)
{
    struct ExprVisitor {
        Generator& gen;
        ExprVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeTerm* term)
        {
            gen.gen_term(term);
        }

        void operator()(const NodeExprBin* expr_bin)
        {
            gen.gen_bin_expr(expr_bin);
        }
    };

    ExprVisitor visitor(*this);
    std::visit(visitor, expr->var);
}

void Generator::gen_stmt(const NodeStmt* stmt)
{
    struct StmtVisitor {
        Generator& gen;
        StmtVisitor(Generator& _gen) :gen(_gen) {}

        void operator()(const NodeStmtCallFunction* call_func)
        {
            // Try to gen void inbuilt func first, then non-void inbuilt func, then defined func
            if (!gen.gen_inbuilt_func(call_func->func, true, false))
            {
                if (gen.gen_inbuilt_func(call_func->func, false, false) || !gen.gen_call_func(call_func->func))
                {
                    gen.pop();
                }
            }
        }

        void operator()(const NodeStmtReturn* stmt_ret)
        {
            // Error check for passing/not passing expression into return
            if (gen.generating_void_function && stmt_ret->expr.has_value())
            {
                compilation_error("Returning expression from void function", stmt_ret->line);
            }

            if (!gen.generating_void_function && !stmt_ret->expr.has_value())
            {
                compilation_error("Return must have expression in non-void functions", stmt_ret->line);
            }

            // Generate expression if there is one
            if (stmt_ret->expr.has_value())
            {
                gen.gen_expr(stmt_ret->expr.value());
            }

            // Remove scope items from stack
            gen.end_scopes_return(stmt_ret->expr.has_value());

            // If there's an expression on the stack, swap it with jump iota
            if (stmt_ret->expr.has_value())
            {
                gen.jesters_gambit();
            }

            // Execute jump iota
            gen.add_pattern(PatternType::hermes_gambit, 0);
        }

        void operator()(const NodeExpr* stmt_expr)
        {
            gen.gen_expr(stmt_expr);
            gen.pop();
        }

        void operator()(const NodeStmtLet* stmt_let)
        {
            if (std::find_if(gen.m_vars.cbegin(), gen.m_vars.cend(), [&](const Var& var){return var.name == stmt_let->ident.value.value();}) != gen.m_vars.cend())
            {
                compilation_error(std::string("Identifier already used: ") + stmt_let->ident.value.value(), stmt_let->line);
            }

            gen.gen_expr(stmt_let->expr);
            gen.m_vars.push_back(Var{.name = stmt_let->ident.value.value(), .stack_loc = gen.m_stack_size - 1, .is_global = false});
        }

        void operator()(const NodeStmtIf* stmt_if)
        {
            // Evaluate expression
            gen.gen_expr(stmt_if->expr);
            gen.augurs_purification();
            --gen.m_stack_size;

            // Generate statement
            gen.add_pattern(PatternType::introspection, 0);
            gen.begin_scope();
            gen.gen_stmt(stmt_if->stmt);
            gen.end_scope();
            gen.add_pattern(PatternType::retrospection, 0);

            // Potentially generate else statement
            if (stmt_if->else_stmt == nullptr)
            {
                gen.vacant_reflection();
                --gen.m_stack_size;
            }
            else
            {
                gen.add_pattern(PatternType::introspection, 0);
                gen.begin_scope();
                gen.gen_stmt(stmt_if->else_stmt);
                gen.end_scope(); 
                gen.add_pattern(PatternType::retrospection, 0);
            }
            
            // Perform bool comparison and execute
            gen.add_pattern(PatternType::augurs_exaltation, 0);
            gen.add_pattern(PatternType::hermes_gambit, 0);
        }

        void operator()(const NodeStmtWhile* stmt_while)
        {
            // Add jump iota to stack for loop
            gen.vacant_reflection();
            gen.add_pattern(PatternType::iris_gambit, 0);

            // Gen condition
            gen.gen_expr(stmt_while->expr);

            // Account for condition not being on stack when generating loop body
            --gen.m_stack_size;

            // If true, gen statements and loop
            gen.add_pattern(PatternType::introspection, 0);
            gen.begin_scope();
            gen.gen_stmt(stmt_while->stmt);
            gen.end_scope();
            gen.gemini_decomposition();
            gen.add_pattern(PatternType::hermes_gambit, 0);
            gen.add_pattern(PatternType::retrospection, 0);

            // If false, do nothing
            gen.vacant_reflection();

            // Actually make comparison and execute
            gen.add_pattern(PatternType::augurs_exaltation, -2);
            gen.add_pattern(PatternType::hermes_gambit, 0);

            // Remove leftover jump iota from stack
            gen.pop();
        }

        void operator()(const NodeScope* stmt_scope)
        {
            gen.begin_scope();

            for (NodeStmt* stmt : stmt_scope->stmts)
            {
                gen.gen_stmt(stmt);
            }

            gen.end_scope();
        }
    };

    StmtVisitor visitor(*this);
    std::visit(visitor, stmt->var);
}

void Generator::gen_func_def(const NodeFunctionDef* func_def)
{
    // Visitor to extract function info
    struct FuncDefVisitor {
        Generator& gen;
        bool& is_void;
        std::vector<Token>& params;
        NodeScope*& scope;
        FuncDefVisitor (Generator& _gen, bool& _is_void, std::vector<Token>& _params, NodeScope*& _scope) :gen(_gen), is_void(_is_void), params(_params), scope(_scope) {}
        
        void operator()(const NodeFunctionDefVoid* func_void)
        {
            is_void = true;
            params = func_void->params;
            scope = func_void->scope;
        }

        void operator()(const NodeFunctionDefRet* func_ret)
        {
            is_void = false;
            params = func_ret->params;
            scope = func_ret->scope;
        }
    };

    // Extract function info
    bool is_void;
    std::vector<Token> params;
    NodeScope* scope;
    FuncDefVisitor visitor(*this, is_void, params, scope);
    std::visit(visitor, func_def->var);

    generating_void_function = is_void;
    m_function_start_scope = m_scopes.size();
    m_function_num_params = params.size();

    add_pattern(PatternType::introspection, 0);
    begin_scope();

    // Treat top of the stack as params
    for (Token param : params)
    {
        m_vars.push_back(Var{.name = param.value.value(), .stack_loc = m_stack_size, .is_global = false});
        ++m_stack_size;
    }

    // Account for jump iota from iris' gambit
    ++m_stack_size;

    // Generate stmts in function
    for (NodeStmt* stmt : scope->stmts)
    {
        gen_stmt(stmt);
    }

    // If function isn't void, then provide null return value by default
    if (is_void)
    {
        end_scope();
    }
    else
    {
        // Remove excess from stack
        end_scopes_return(false);
        // Destroy jump iota
        pop();
        // Add null to stack
        nullary_reflection();
    }
    add_pattern(PatternType::retrospection, 1);
}

void Generator::gen_prog()
{
    // Gen global var exprs
    for (NodeGlobalLet* global_let : m_prog->vars)
    {
        if (std::find_if(m_global_vars.cbegin(), m_global_vars.cend(), [&](const Var& var){return var.name == global_let->ident.value.value();}) != m_global_vars.cend())
        {
            compilation_error(std::string("Global identifier already used: ") + global_let->ident.value.value(), global_let->line);
        }

        gen_expr(global_let->expr);

        // Register temporarily as local var so they can reference other global vars during declaration
        m_vars.push_back(Var{.name = global_let->ident.value.value(), .stack_loc = m_vars.size(), .is_global = false});
    }

    // Clear temp local vars
    m_vars.clear();

    // Mark global variables as declared
    for (NodeGlobalLet* global_let : m_prog->vars)
    {
        m_global_vars.push_back(Var{.name = global_let->ident.value.value(), .stack_loc = m_global_vars.size(), .is_global = true});
    }

    // Account for main's jump iota
    ++m_stack_size;

    // Visitor for declaring functions
    struct FuncDefVisitor {
        Generator& gen;
        FuncDefVisitor (Generator& _gen) :gen(_gen) {}
        
        void operator()(const NodeFunctionDefVoid* func_void)
        {
            gen.dec_func(true, func_void->ident.value.value(), func_void->params.size(), func_void->line);
        }

        void operator()(const NodeFunctionDefRet* func_ret)
        {
            gen.dec_func(false, func_ret->ident.value.value(), func_ret->params.size(), func_ret->line);
        }
    };
    FuncDefVisitor visitor(*this);

    // Gen function declarations
    for (NodeFunctionDef* func_def : m_prog->funcs)
    {
        std::visit(visitor, func_def->var);
    }

    // Gen functions
    for (NodeFunctionDef* func_def : m_prog->funcs)
    {
        gen_func_def(func_def);
    }

    // Store functions and global vars in list in raven's mind
    if (m_global_vars.size() + m_funcs.size() > 0)
    {
        if (m_global_vars.size() + m_funcs.size() == 1)
        {
            singles_purification();
        }
        else
        {
            numerical_reflection(std::to_string(m_global_vars.size() + m_funcs.size()));
            flocks_gambit(m_global_vars.size() + m_funcs.size());
        }

        huginns_gambit();
    }

    // Remove accounting of jump iota so it isn't double counted
    --m_stack_size;

    // Gen main
    gen_func_def(m_prog->main_);

    // Execute main
    add_pattern(PatternType::iris_gambit, -1);
}



void Generator::try_gen_x_exprs(std::vector<NodeExpr*> exprs, int correct_amount, size_t line)
{
    if (exprs.size() != correct_amount)
    {
        compilation_error("Incorrect number of arguments passed into function", line);
    }

    for (NodeExpr* expr : exprs)
    {
        gen_expr(expr);
    }
}

void Generator::pop(int amount)
{
    if (amount <= 0)
    {
        return;
    }

    add_pattern(PatternType::bookkeepers_gambit, -amount, std::string(amount, 'v'));
}

void Generator::begin_scope()
{
    m_scopes.push_back(Scope{.stack_size = m_stack_size, .var_num = m_vars.size()});
}

void Generator::end_scope()
{
    size_t pop_count = m_stack_size - m_scopes.back().stack_size;
    pop(pop_count);

    m_vars.resize(m_scopes.back().var_num);

    m_scopes.pop_back();
}

void Generator::end_scopes_return(bool has_ret_value)
{
    size_t pop_count = m_stack_size - m_scopes[m_function_start_scope].stack_size - (has_ret_value ? 1 : 0) - 1;

    // Bookkeepr's Gambit all stack elements except possible ret value and jump iota
    add_pattern(PatternType::bookkeepers_gambit, 0,
        // Preserve parameters if they exist
        (m_function_num_params != 0 ? (std::string(m_function_num_params, 'v') + '-') : std::string()) +
        // Pop rest of scope
        std::string(pop_count - m_function_num_params, 'v') +
        // Preserve return value if it exists
        (has_ret_value ? "-" : ""));
}

void Generator::dec_func(bool is_void, std::string name, int num_params, size_t line)
{
    if (std::find_if(m_funcs.cbegin(), m_funcs.cend(), [&](const Func& func){return func.name == name && func.num_params == num_params;}) != m_funcs.cend())
    {
        compilation_error(std::string("Function with this name and number of parameters already declared: ") + name, line);
    }

    m_funcs.push_back(Func{.is_void = is_void, .name = name, .num_params = num_params, .stack_loc = m_global_vars.size() + m_funcs.size()});
}



void Generator::akashas_distillation()
{
    add_pattern(PatternType::akashas_distillation, -1);
}

void Generator::akashas_gambit()
{
    add_pattern(PatternType::akashas_gambit, -3);
}

void Generator::additive_distillation()
{
    add_pattern(PatternType::additive_distillation, -1);
}

void Generator::anchorites_flight()
{
    add_pattern(PatternType::anchorites_flight, -2);
}

void Generator::alidades_purification()
{
    add_pattern(PatternType::alidades_purification, 0);
}

void Generator::aviators_purification()
{
    add_pattern(PatternType::aviators_purification, 0);
}

void Generator::archers_distillation()
{
    add_pattern(PatternType::archers_distillation, -1);
}

void Generator::architects_distillation()
{
    add_pattern(PatternType::architects_distillation, -1);
}

void Generator::arcs_reflection()
{
    add_pattern(PatternType::arcs_reflection, 1);
}

void Generator::assessors_purification()
{
    add_pattern(PatternType::assessors_purification, 0);
}

void Generator::assessors_reflection()
{
    add_pattern(PatternType::assessors_reflection, 1);
}

void Generator::auditors_purification()
{
    add_pattern(PatternType::auditors_purification, 0);
}

void Generator::auditors_reflection()
{
    add_pattern(PatternType::auditors_reflection, 1);
}

void Generator::augurs_exaltation()
{
    add_pattern(PatternType::augurs_exaltation, -2);
}

void Generator::augurs_purification()
{
    add_pattern(PatternType::augurs_purification, 0);
}

void Generator::axial_purification()
{
    add_pattern(PatternType::axial_purification, 0);
}

void Generator::black_suns_nadir()
{
    add_pattern(PatternType::black_suns_nadir, -3);
}

void Generator::black_suns_zenith()
{
    add_pattern(PatternType::black_suns_zenith, -3);
}

void Generator::blue_suns_nadir()
{
    add_pattern(PatternType::blue_suns_nadir, -2);
}

void Generator::blue_suns_zenith()
{
    add_pattern(PatternType::blue_suns_zenith, -2);
}

void Generator::bookkeepers_gambit(std::string value)
{
    add_pattern(PatternType::bookkeepers_gambit, -std::count(value.cbegin(), value.cend(), 'v'), value);
}

void Generator::break_block()
{
    add_pattern(PatternType::break_block, -1);
}

void Generator::ceiling_purification()
{
    add_pattern(PatternType::ceiling_purification, 0);
}

void Generator::chroniclers_gambit()
{
    add_pattern(PatternType::chroniclers_gambit, -2);
}

void Generator::chroniclers_prfn()
{
    add_pattern(PatternType::chroniclers_prfn, 0);
}

void Generator::chroniclers_purification()
{
    add_pattern(PatternType::chroniclers_prfn, 0);
}

void Generator::circle_reflection()
{
    add_pattern(PatternType::circle_reflection, 1);
}

void Generator::compass_purification()
{
    add_pattern(PatternType::compass_purification, 0);
}

void Generator::compass_purification_II()
{
    add_pattern(PatternType::compass_purification_II, 0);
}

void Generator::cosine_purification()
{
    add_pattern(PatternType::cosine_purification, 0);
}

void Generator::craft_phial()
{
    add_pattern(PatternType::craft_phial, -1);
}

void Generator::conjunction_distillation()
{
    add_pattern(PatternType::conjunction_distillation, -1);
}

void Generator::conjure_light()
{
    add_pattern(PatternType::conjure_light, -1);
}

void Generator::create_water()
{
    add_pattern(PatternType::create_water, -1);
}

void Generator::derivation_decomposition()
{
    add_pattern(PatternType::derivation_decomposition, 1);
}

void Generator::dioscuri_gambit()
{
    add_pattern(PatternType::dioscuri_gambit, 2);
}

void Generator::disjunction_distillation()
{
    add_pattern(PatternType::disjunction_distillation, -1);
}

void Generator::division_distillation()
{
    add_pattern(PatternType::division_distillation, -1);
}

void Generator::dispel_rain()
{
    add_pattern(PatternType::dispel_rain, 0);
}

void Generator::entropy_reflection()
{
    add_pattern(PatternType::entropy_reflection, 1);
}

void Generator::equality_distillation()
{
    add_pattern(PatternType::equality_distillation, -1);
}

void Generator::eulers_reflection()
{
    add_pattern(PatternType::eulers_reflection, 1);
}

void Generator::entity_prfn()
{
    add_pattern(PatternType::entity_prfn, 0);
}

void Generator::entity_prfn_animal()
{
    add_pattern(PatternType::entity_prfn_animal, 0);
}

void Generator::entity_prfn_item()
{
    add_pattern(PatternType::entity_prfn_item, 0);
}

void Generator::entity_prfn_living()
{
    add_pattern(PatternType::entity_prfn_living, 0);
}

void Generator::entity_prfn_monster()
{
    add_pattern(PatternType::entity_prfn_monster, 0);
}

void Generator::entity_prfn_player()
{
    add_pattern(PatternType::entity_prfn_player, 0);
}

void Generator::excisors_distillation()
{
    add_pattern(PatternType::excisors_distillation, -1);
}

void Generator::exclusion_distillation()
{
    add_pattern(PatternType::exclusion_distillation, -1);
}

void Generator::explosion()
{
    add_pattern(PatternType::explosion, -2);
}

void Generator::false_reflection()
{
    add_pattern(PatternType::false_reflection, 1);
}

void Generator::fireball()
{
    add_pattern(PatternType::fireball, -2);
}

void Generator::fishermans_gambit()
{
    add_pattern(PatternType::fishermans_gambit, -1);
}

void Generator::fishermans_gambit_II()
{
    add_pattern(PatternType::fishermans_gambit_II, 0);
}

void Generator::flocks_gambit(int num_iotas_packed)
{
    add_pattern(PatternType::flocks_gambit, -num_iotas_packed);
}

void Generator::flocks_reflection()
{
    add_pattern(PatternType::flocks_reflection, 1);
}

void Generator::floor_purification()
{
    add_pattern(PatternType::floor_purification, 0);
}

void Generator::gemini_decomposition()
{
    add_pattern(PatternType::gemini_decomposition, 1);
}

void Generator::greater_fold_reflection()
{
    add_pattern(PatternType::greater_fold_reflection, 1);
}

void Generator::huginns_gambit()
{
    add_pattern(PatternType::huginns_gambit, -1);
}

void Generator::inequality_distillation()
{
    add_pattern(PatternType::inequality_distillation, -1);
}

void Generator::integration_distillation()
{
    add_pattern(PatternType::integration_distillation, -1);
}

void Generator::inverse_cosine_purification()
{
    add_pattern(PatternType::inverse_cosine_purification, 0);
}

void Generator::inverse_sine_purification()
{
    add_pattern(PatternType::inverse_sine_purification, 0);
}

void Generator::inverse_tangent_distillation()
{
    add_pattern(PatternType::inverse_tangent_distillation, -1);
}

void Generator::inverse_tangent_purification()
{
    add_pattern(PatternType::inverse_tangent_purification, 0);
}

void Generator::jesters_gambit()
{
    add_pattern(PatternType::jesters_gambit, 0);
}

void Generator::length_purification()
{
    add_pattern(PatternType::length_purification, 0);
}

void Generator::lesser_fold_reflection()
{
    add_pattern(PatternType::lesser_fold_reflection, 1);
}

void Generator::locators_distillation()
{
    add_pattern(PatternType::locators_distillation, -1);
}

void Generator::lodestone_reflection()
{
    add_pattern(PatternType::lodestone_reflection, 1);
}

void Generator::logarithmic_distillation()
{
    add_pattern(PatternType::logarithmic_distillation, -1);
}

void Generator::make_note()
{
    add_pattern(PatternType::make_note, -3);
}

void Generator::maximus_distillation()
{
    add_pattern(PatternType::maximus_distillation, -1);
}

void Generator::maximus_distillation_II()
{
    add_pattern(PatternType::maximus_distillation_II, -1);
}

void Generator::minds_reflection()
{
    add_pattern(PatternType::minds_reflection, 1);
}

void Generator::minimus_distillation()
{
    add_pattern(PatternType::minimus_distillation, -1);
}

void Generator::minimus_distillation_II()
{
    add_pattern(PatternType::minimus_distillation_II, -1);
}

void Generator::modulus_distillation()
{
    add_pattern(PatternType::modulus_distillation, -1);
}

void Generator::multiplicative_distillation()
{
    add_pattern(PatternType::multiplicative_distillation, -1);
}

void Generator::muninns_reflection()
{
    add_pattern(PatternType::muninns_reflection, 1);
}

void Generator::negation_purification()
{
    add_pattern(PatternType::negation_purification, 0);
}

void Generator::nullary_reflection()
{
    add_pattern(PatternType::nullary_reflection, 1);
}

void Generator::numerical_reflection(std::string value)
{
    if (value.find('.') != std::string::npos)
    {
        has_non_integer_num = true;
    }

    add_pattern(PatternType::numerical_reflection, 1, value);
}

void Generator::pace_purification()
{
    add_pattern(PatternType::pace_purification, 0);
}

void Generator::power_distillation()
{
    add_pattern(PatternType::power_distillation, -1);
}

void Generator::prospectors_gambit()
{
    add_pattern(PatternType::prospectors_gambit, 1);
}

void Generator::retrograde_purification()
{
    add_pattern(PatternType::retrograde_purification, 0);
}

void Generator::reveal()
{
    add_pattern(PatternType::reveal, 0);
}

void Generator::rotation_gambit()
{
    add_pattern(PatternType::rotation_gambit, 0);
}

void Generator::rotation_gambit_II()
{
    add_pattern(PatternType::rotation_gambit_II, 0);
}

void Generator::scouts_distillation()
{
    add_pattern(PatternType::scouts_distillation, -1);
}

void Generator::scribes_gambit()
{
    add_pattern(PatternType::scribes_gambit, -1);
}

void Generator::scribes_reflection()
{
    add_pattern(PatternType::scribes_reflection, 1);
}

void Generator::selection_distillation()
{
    add_pattern(PatternType::selection_distillation, -1);
}

void Generator::selection_exaltation()
{
    add_pattern(PatternType::selection_exaltation, -2);
}

void Generator::singles_purification()
{
    add_pattern(PatternType::singles_purification, 0);
}

void Generator::speakers_decomposition()
{
    add_pattern(PatternType::speakers_decomposition, 1);
}

void Generator::speakers_distillation()
{
    add_pattern(PatternType::speakers_distillation, -1);
}

void Generator::stadiometers_purification()
{
    add_pattern(PatternType::stadiometers_prfn, 0);
}

void Generator::subtractive_distillation()
{
    add_pattern(PatternType::subtractive_distillation, -1);
}

void Generator::sine_purification()
{
    add_pattern(PatternType::sine_purification, 0);
}

void Generator::stadiometers_prfn()
{
    add_pattern(PatternType::stadiometers_prfn, 0);
}

void Generator::swindlers_gambit()
{
    add_pattern(PatternType::swindlers_gambit, -1);
}

void Generator::summon_lightning()
{
    add_pattern(PatternType::summon_lightning, -1);
}

void Generator::summon_rain()
{
    add_pattern(PatternType::summon_rain, 0);
}

void Generator::surgeons_exaltation()
{
    add_pattern(PatternType::surgeons_exaltation, -2);
}

void Generator::tangent_purification()
{
    add_pattern(PatternType::tangent_purification, 0);
}

void Generator::thanatos_reflection()
{
    add_pattern(PatternType::thanatos_reflection, 1);
}

void Generator::true_reflection()
{
    add_pattern(PatternType::true_reflection, 1);
}

void Generator::uniqueness_purification()
{
    add_pattern(PatternType::uniqueness_purification, 0);
}

void Generator::vacant_reflection()
{
    // This uses no patterns but achieves the same effect as vacant reflection
    add_pattern(PatternType::introspection, 0);
    add_pattern(PatternType::retrospection, 1);
    //add_pattern(PatternType::vacant_reflection, 1);
}

void Generator::vector_disintegration()
{
    add_pattern(PatternType::vector_disintegration, 2);
}

void Generator::vector_exaltation()
{
    add_pattern(PatternType::vector_exaltation, -2);
}

void Generator::vector_reflection_NX()
{
    add_pattern(PatternType::vector_reflection_NX, 1);
}

void Generator::vector_reflection_NY()
{
    add_pattern(PatternType::vector_reflection_NY, 1);
}

void Generator::vector_reflection_NZ()
{
    add_pattern(PatternType::vector_reflection_NZ, 1);
}

void Generator::vector_reflection_PX()
{
    add_pattern(PatternType::vector_reflection_PX, 1);
}

void Generator::vector_reflection_PY()
{
    add_pattern(PatternType::vector_reflection_PY, 1);
}

void Generator::vector_reflection_PZ()
{
    add_pattern(PatternType::vector_reflection_PZ, 1);
}

void Generator::vector_reflection_zero()
{
    add_pattern(PatternType::vector_reflection_zero, 1);
}

void Generator::zone_dstl_animal()
{
    add_pattern(PatternType::zone_dstl_animal, -1);
}

void Generator::zone_dstl_any()
{
    add_pattern(PatternType::zone_dstl_any, -1);
}

void Generator::zone_dstl_item()
{
    add_pattern(PatternType::zone_dstl_item, -1);
}

void Generator::zone_dstl_living()
{
    add_pattern(PatternType::zone_dstl_living, -1);
}

void Generator::zone_dstl_monster()
{
    add_pattern(PatternType::zone_dstl_monster, -1);
}

void Generator::zone_dstl_non_animal()
{
    add_pattern(PatternType::zone_dstl_non_animal, -1);
}

void Generator::zone_dstl_non_item()
{
    add_pattern(PatternType::zone_dstl_non_item, -1);
}

void Generator::zone_dstl_non_living()
{
    add_pattern(PatternType::zone_dstl_non_living, -1);
}

void Generator::zone_dstl_non_monster()
{
    add_pattern(PatternType::zone_dstl_non_monster, -1);
}

void Generator::zone_dstl_non_player()
{
    add_pattern(PatternType::zone_dstl_non_player, -1);
}

void Generator::zone_dstl_player()
{
    add_pattern(PatternType::zone_dstl_player, -1);
}

void Generator::altiora()
{
    add_pattern(PatternType::altiora, -1);
}

void Generator::banish_sentinel()
{
    add_pattern(PatternType::banish_sentinel, 0);
}

void Generator::blink()
{
    add_pattern(PatternType::blink, -2);
}

void Generator::casters_glamour()
{
    add_pattern(PatternType::casters_glamour, 0);
}

void Generator::conjure_block()
{
    add_pattern(PatternType::conjure_block, -1);
}

void Generator::craft_artifact()
{
    add_pattern(PatternType::craft_artifact, -2);
}

void Generator::craft_cypher()
{
    add_pattern(PatternType::craft_cypher, -2);
}

void Generator::craft_trinket()
{
    add_pattern(PatternType::craft_trinket, -2);
}

void Generator::create_lava()
{
    add_pattern(PatternType::create_lava, -1);
}

void Generator::destroy_liquid()
{
    add_pattern(PatternType::destroy_liquid, -1);
}

void Generator::edify_sapling()
{
    add_pattern(PatternType::edify_sapling, -1);
}

void Generator::erase_item()
{
    add_pattern(PatternType::erase_item, 0);
}

void Generator::extinguish_area()
{
    add_pattern(PatternType::extinguish_area, -1);
}

void Generator::flay_mind()
{
    add_pattern(PatternType::flay_mind, -2);
}

void Generator::greater_teleport()
{
    add_pattern(PatternType::greater_teleport, -2);
}

void Generator::green_suns_nadir()
{
    add_pattern(PatternType::green_suns_nadir, -3);
}

void Generator::green_suns_zenith()
{
    add_pattern(PatternType::green_suns_zenith, -3);
}

void Generator::ignite()
{
    add_pattern(PatternType::ignite, -1);
}

void Generator::impulse()
{
    add_pattern(PatternType::impulse, -2);
}

void Generator::internalize_pigment()
{
    add_pattern(PatternType::internalize_pigment, 0);
}

void Generator::locate_sentinel()
{
    add_pattern(PatternType::locate_sentinel, 1);
}

void Generator::overgrow()
{
    add_pattern(PatternType::overgrow, -1);
}

void Generator::place_block()
{
    add_pattern(PatternType::place_block, -1);
}

void Generator::recharge_item()
{
    add_pattern(PatternType::recharge_item, -1);
}

void Generator::red_suns_nadir()
{
    add_pattern(PatternType::red_suns_nadir, -3);
}

void Generator::red_suns_zenith()
{
    add_pattern(PatternType::red_suns_zenith, -3);
}

void Generator::summon_greater_sentinel()
{
    add_pattern(PatternType::summon_greater_sentinel, -1);
}

void Generator::summon_sentinel()
{
    add_pattern(PatternType::summon_sentinel, -1);
}

void Generator::wayfind_sentinel()
{
    add_pattern(PatternType::wayfind_sentinel, 0);
}

void Generator::wayfarers_flight()
{
    add_pattern(PatternType::wayfarers_flight, -2);
}

void Generator::waystone_reflection()
{
    add_pattern(PatternType::waystone_reflection, 1);
}

void Generator::white_suns_nadir()
{
    add_pattern(PatternType::white_suns_nadir, -3);
}

void Generator::white_suns_zenith()
{
    add_pattern(PatternType::white_suns_zenith, -3);
}

void Generator::add_pattern(PatternType pattern_type, size_t stack_size_net, std::optional<std::string> value)
{
    m_output.push_back(Pattern{.type = pattern_type, .value = value});
    m_stack_size += stack_size_net;
}