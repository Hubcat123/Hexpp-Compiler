#pragma once

#include "parser.hpp"

#include <sstream>
#include <stack>

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
    num_patterns
};

struct Pattern {
    PatternType type;
    std::optional<std::string> value;
};



class Generator {
public:
    struct Var {
        std::string name;
        size_t stack_loc;
        bool is_global;
    };

    Generator(const NodeProg* root);

    std::vector<Pattern> generate();

    void gen_assignment(const NodeTermVar* var, const std::variant<const NodeExpr*, const float> value, TokenType_ op, size_t line, bool is_post = false);
    bool gen_inbuilt_func(const NodeDefinedFunc* func, bool is_void, bool is_member);
    // Returns whether func is void func
    bool gen_call_func(const NodeDefinedFunc* func);
    void gen_bin_expr(const NodeExprBin* expr_bin);
    // Returns the var of the variable gen-ed
    Var gen_var_ident(const std::string ident_name, size_t line, bool dont_gen_if_global = false, bool leave_copy = true);
    void gen_var(const NodeTermVar* var);
    void gen_term(const NodeTerm* term);
    void gen_expr(const NodeExpr* expr);
    void gen_stmt(const NodeStmt* stmt);
    void gen_func_def(const NodeFunctionDef* func_def);
    void gen_prog();
    
    void try_gen_x_exprs(std::vector<NodeExpr*> exprs, int correct_amount, size_t line);
    void pop(int amount = 1);
    void begin_scope();
    void end_scope();
    void end_scopes_return(bool has_ret_value);
    void dec_func(bool is_void, std::string name, int num_params, size_t line);

    void akashas_distillation();
    void akashas_gambit();
    void altiora();
    void additive_distillation();
    void anchorites_flight();
    void alidades_purification();
    void aviators_purification();
    void banish_sentinel();
    void archers_distillation();
    void architects_distillation();
    void arcs_reflection();
    void assessors_purification();
    void assessors_reflection();
    void auditors_purification();
    void auditors_reflection();
    void augurs_exaltation();
    void augurs_purification();
    void axial_purification();
    void black_suns_nadir();
    void black_suns_zenith();
    void blink();
    void blue_suns_nadir();
    void blue_suns_zenith();
    void bookkeepers_gambit(std::string value);
    void break_block();
    void ceiling_purification();
    void charons_gambit();
    void casters_glamour();
    void chroniclers_gambit();
    void chroniclers_prfn();
    void chroniclers_purification();
    void circle_reflection();
    void compass_purification();
    void compass_purification_II();
    void cosine_purification();
    void craft_artifact();
    void craft_cypher();
    void craft_trinket();
    void craft_phial();
    // AND
    void conjunction_distillation();
    void conjure_light();
    void conjure_block();
    void create_water();
    void create_lava();
    void derivation_decomposition();
    void destroy_liquid();
    void dioscuri_gambit();
    // OR
    void disjunction_distillation();
    void division_distillation();
    void dispel_rain();
    void entropy_reflection();
    void equality_distillation();
    void erase_item();
    void extinguish_area();
    void eulers_reflection();
    void edify_sapling();
    void entity_prfn();
    void entity_prfn_animal();
    void entity_prfn_item();
    void entity_prfn_living();
    void entity_prfn_monster();
    void entity_prfn_player();
    void excisors_distillation();
    void exclusion_distillation();
    void explosion();
    void false_reflection();
    void fireball();
    void fishermans_gambit();
    void fishermans_gambit_II();
    void flocks_gambit(int num_iotas_packed);
    void flocks_reflection();
    void flay_mind();
    void floor_purification();
    void gemini_decomposition();
    void green_suns_nadir();
    void green_suns_zenith();
    void greater_fold_reflection();
    void greater_teleport();
    // Store in Raven's Mind
    void huginns_gambit();
    void ignite();
    void inequality_distillation();
    void impulse();
    void integration_distillation();
    void inverse_cosine_purification();
    void inverse_sine_purification();
    void inverse_tangent_distillation();
    void inverse_tangent_purification();
    void internalize_pigment();
    void jesters_gambit();
    void length_purification();
    void locate_sentinel();
    void lesser_fold_reflection();
    void locators_distillation();
    void lodestone_reflection();
    void logarithmic_distillation();
    void make_note();
    void maximus_distillation();
    void maximus_distillation_II();
    void minds_reflection();
    void minimus_distillation();
    void minimus_distillation_II();
    void modulus_distillation();
    void multiplicative_distillation();
    // Retrieve from Raven's Mind
    void muninns_reflection();
    void negation_purification();
    void nullary_reflection();
    void numerical_reflection(std::string value);
    void overgrow();
    void pace_purification();
    void place_block();
    void power_distillation();
    void prospectors_gambit();
    void retrograde_purification();
    void recharge_item();
    void reveal();
    void red_suns_nadir();
    void red_suns_zenith();
    // 3rd becomes 1st
    void rotation_gambit();
    // 1st becomes 3rd
    void rotation_gambit_II();
    void scouts_distillation();
    void scribes_gambit();
    void scribes_reflection();
    void selection_distillation();
    void selection_exaltation();
    void singles_purification();
    void speakers_decomposition();
    void speakers_distillation();
    void stadiometers_purification();
    void subtractive_distillation();
    void sine_purification();
    void stadiometers_prfn();
    void swindlers_gambit();
    void summon_lightning();
    void summon_rain();
    void summon_greater_sentinel();
    void summon_sentinel();
    void surgeons_exaltation();
    void tangent_purification();
    void thanatos_reflection();
    void true_reflection();
    void uniqueness_purification();
    void vacant_reflection();
    void vector_disintegration();
    void vector_exaltation();
    void vector_reflection_NX();
    void vector_reflection_NY();
    void vector_reflection_NZ();
    void vector_reflection_PX();
    void vector_reflection_PY();
    void vector_reflection_PZ();
    void vector_reflection_zero();
    void zone_dstl_animal();
    void zone_dstl_any();
    void zone_dstl_item();
    void zone_dstl_living();
    void zone_dstl_monster();
    void zone_dstl_non_animal();
    void zone_dstl_non_item();
    void zone_dstl_non_living();
    void zone_dstl_non_monster();
    void zone_dstl_non_player();
    void zone_dstl_player();
    void wayfarers_flight();
    void wayfind_sentinel();
    void waystone_reflection();
    void white_suns_nadir();
    void white_suns_zenith();

    void add_pattern(PatternType pattern_type, size_t stack_size_net, std::optional<std::string> value = std::nullopt);

    bool has_non_integer_num = false;

    bool generating_void_function = false;
private:

    struct Func {
        bool is_void;
        std::string name;
        int num_params;
        size_t stack_loc;
    };

    struct Scope {
        size_t stack_size;
        size_t var_num;
    };

    const NodeProg* m_prog;
    std::vector<Pattern> m_output;
    size_t m_stack_size = 0;
    std::vector<Var> m_vars {};
    std::vector<Var> m_global_vars {};
    std::vector<Func> m_funcs {};
    std::vector<Scope> m_scopes {};

    size_t m_function_start_scope;
    size_t m_function_num_params;
};